#include "Saida.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include <cstdio>
#include <cmath>

namespace fs = std::filesystem;

#ifdef _WIN32
  #include <windows.h>
  #include <gdiplus.h>

  static CLSID GetEncoderClsid(const WCHAR* format) {
      UINT num = 0, size = 0;
      Gdiplus::GetImageEncodersSize(&num, &size);
      if (size == 0) return CLSID{};

      auto p = static_cast<Gdiplus::ImageCodecInfo*>(malloc(size));
      if (!p) return CLSID{};

      Gdiplus::GetImageEncoders(num, size, p);

      CLSID clsid{};
      for (UINT j = 0; j < num; ++j) {
          if (wcscmp(p[j].MimeType, format) == 0) {
              clsid = p[j].Clsid;
              break;
          }
      }
      free(p);
      return clsid;
  }

  static std::wstring widen(const std::string& s) {
      if (s.empty()) return L"";
      int len = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
      std::wstring ws(len, L'\0');
      MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, ws.data(), len);
      if (!ws.empty() && ws.back() == L'\0') ws.pop_back();
      return ws;
  }
#endif

void Saida::configurarDiretorio(const std::string& prefixo, const std::string& suapId) {
    suapId_ = suapId;
    std::cout << "[CONFIG] Diretório configurado: " << prefixo << suapId_ << std::endl;
}

void Saida::salvarSequencialM3(const Medicao& m, const Display& d) {
    (void)d; // não usado
    if (!salvarImagem_) {
        std::cout << "[SAIDA] Salvamento de imagem desabilitado." << std::endl;
        return;
    }

    try {
        // Incrementa contador de imagem (1 a 99, depois cicla)
        contadorImagem_++;
        if (contadorImagem_ > 99) contadorImagem_ = 1;
        
        int numeroImagem = contadorImagem_;
        double volumeAtual = m.getVolumeTotal_m3();

        // Cria diretório base
        fs::path base = fs::path(caminhoArquivo_).parent_path();
        if (base.empty()) base = fs::current_path();

        // Diretório das medições
        fs::path pasta = base / (std::string("Medicoes_") + suapId_);
        std::error_code ec;
        fs::create_directories(pasta, ec);
        if (ec) {
            std::cerr << "[ERRO] Falha ao criar diretório: " << pasta << " - " << ec.message() << std::endl;
            return;
        }

        // Nome do arquivo
        char nomeArquivo[16];
        std::snprintf(nomeArquivo, sizeof(nomeArquivo), "%02d.jpeg", numeroImagem);
        fs::path destino = pasta / nomeArquivo;

        std::cout << "[SAIDA] Salvando: " << destino.string() << std::endl;

#ifdef _WIN32
        salvarJPEG(m, destino.string());
        std::cout << "[SAIDA] ✅ Imagem salva: " << destino.filename() 
                  << " (Volume: " << volumeAtual << " m³)" << std::endl;
#else
        std::cerr << "[SAIDA] ❌ Geração de imagens disponível apenas no Windows." << std::endl;
#endif

    } catch (const std::exception& e) {
        std::cerr << "[ERRO] Falha ao salvar imagem: " << e.what() << std::endl;
    }
}

#ifdef _WIN32
void Saida::salvarJPEG(const Medicao& m, const std::string& caminhoCompleto) const {
    if (baseImagem_.empty()) {
        std::cerr << "[SAIDA] BASE_IMG não definido." << std::endl;
        return;
    }

    ULONG_PTR token{};
    Gdiplus::GdiplusStartupInput input;
    if (Gdiplus::GdiplusStartup(&token, &input, nullptr) != Gdiplus::Ok) {
        std::cerr << "[SAIDA] Falha ao iniciar GDI+." << std::endl;
        return;
    }

    // Carrega a imagem base
    Gdiplus::Image base(widen(baseImagem_).c_str());
    if (base.GetLastStatus() != Gdiplus::Ok) {
        std::cerr << "[SAIDA] Não foi possível abrir a base: " << baseImagem_ << std::endl;
        Gdiplus::GdiplusShutdown(token);
        return;
    }

    // Cria canvas
    Gdiplus::Bitmap canvas(base.GetWidth(), base.GetHeight(), PixelFormat32bppARGB);
    Gdiplus::Graphics g(&canvas);
    g.DrawImage(&base, 0, 0);

    // Configurações de qualidade
    g.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
    g.SetTextRenderingHint(Gdiplus::TextRenderingHintClearTypeGridFit);

    // Tamanho da base
    const float W = static_cast<float>(base.GetWidth());
    const float H = static_cast<float>(base.GetHeight());

    // Fonte proporcional ao tamanho da imagem
    Gdiplus::FontFamily ff(L"Consolas");
    Gdiplus::Font fontMain (&ff, 0.050f * H, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
    Gdiplus::Font fontSmall(&ff, 0.035f * H, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
    Gdiplus::StringFormat sf; 
    sf.SetAlignment(Gdiplus::StringAlignmentCenter);
    sf.SetLineAlignment(Gdiplus::StringAlignmentCenter);
    
    Gdiplus::SolidBrush preto(Gdiplus::Color(0, 0, 0));
    Gdiplus::SolidBrush vermelho(Gdiplus::Color(220, 0, 0));

    // Cálculo dos dígitos
    int inteiro_m3 = static_cast<int>(m.getVolumeTotal_m3()) % 100000;
    int mmVal = m.getMM(); // litros (0-999)

    // Array de 6 dígitos para o odômetro principal
    int d[6] = {0, 0, 0, 0, 0, 0};
    
    // Primeiros 4 dígitos: metros cúbicos (pretos)
    int m3_4 = inteiro_m3 % 10000;
    for (int i = 3; i >= 0; --i) { 
        d[i] = m3_4 % 10; 
        m3_4 /= 10; 
    }
    
    // Últimos 2 dígitos: centena e dezena de litros (vermelhos)
    d[4] = (mmVal / 100) % 10; // centenas de litros
    d[5] = (mmVal / 10) % 10;  // dezenas de litros

    // Desenha odômetro principal - coordenadas relativas para qualquer tamanho
    const float mainX = 0.336f * W;   // posição X inicial
    const float mainY = 0.348f * H;   // posição Y
    const float boxW  = 0.043f * W;   // largura de cada dígito
    const float boxH  = 0.120f * H;   // altura
    const float gap   = 0.012f * W;   // espaçamento entre dígitos

    for (int i = 0; i < 6; ++i) {
        Gdiplus::RectF r(mainX + i * (boxW + gap), mainY, boxW, boxH);
        wchar_t ch[2] = { wchar_t(L'0' + d[i]), 0 };
        Gdiplus::SolidBrush& br = (i >= 4) ? vermelho : preto;
        g.DrawString(ch, 1, &fontMain, r, &sf, &br);
    }

    // Bloco "L" (litros) - 2 dígitos pretos (unidades e décimos de litros)
    {
        const float lx   = 0.578f * W;
        const float ly   = 0.520f * H;
        const float lbW  = 0.043f * W;
        const float lbH  = 0.100f * H;
        const float lgap = 0.012f * W;

        int L2 = mmVal % 100; // últimos 2 dígitos dos litros
        int a = L2 / 10, b = L2 % 10;
        
        Gdiplus::RectF r1(lx, ly, lbW, lbH);
        Gdiplus::RectF r2(lx + lbW + lgap, ly, lbW, lbH);
        
        wchar_t ca[2] = { wchar_t(L'0' + a), 0 };
        wchar_t cb[2] = { wchar_t(L'0' + b), 0 };
        
        g.DrawString(ca, 1, &fontSmall, r1, &sf, &preto);
        g.DrawString(cb, 1, &fontSmall, r2, &sf, &preto);
    }

    // Bloco "dm³" (decímetros cúbicos) - 2 dígitos pretos
    {
        const float dx   = 0.455f * W;
        const float dy   = 0.587f * H;
        const float dbW  = 0.043f * W;
        const float dbH  = 0.100f * H;
        const float dgap = 0.012f * W;

        // Calcula decímetros cúbicos baseado na parte fracionária do volume
        double volumeTotal = m.getVolumeTotal_m3();
        int decimetros = static_cast<int>((volumeTotal * 10000.0)) % 100;
        
        int a = decimetros / 10, b = decimetros % 10;
        
        Gdiplus::RectF r1(dx, dy, dbW, dbH);
        Gdiplus::RectF r2(dx + dbW + dgap, dy, dbW, dbH);
        
        wchar_t ca[2] = { wchar_t(L'0' + a), 0 };
        wchar_t cb[2] = { wchar_t(L'0' + b), 0 };
        
        g.DrawString(ca, 1, &fontSmall, r1, &sf, &preto);
        g.DrawString(cb, 1, &fontSmall, r2, &sf, &preto);
    }

    // Salva como JPEG
    auto wout = widen(caminhoCompleto);
    CLSID enc = GetEncoderClsid(L"image/jpeg");
    if (enc == CLSID{}) {
        std::cerr << "[SAIDA] Encoder JPEG não encontrado." << std::endl;
        Gdiplus::GdiplusShutdown(token);
        return;
    }

    if (canvas.Save(wout.c_str(), &enc, nullptr) != Gdiplus::Ok) {
        std::cerr << "[SAIDA] Falha ao salvar JPEG: " << caminhoCompleto << std::endl;
    }

    Gdiplus::GdiplusShutdown(token);
}
#endif
