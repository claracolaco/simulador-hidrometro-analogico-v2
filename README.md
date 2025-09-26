# Simulador de Hidrômetro Analógico 2.0

## Ferramenta utilizada para o Diagrama de Classes — SHA 2.0

- Baixe/visualize o diagrama: [Clique aqui](Diagrama%20de%20classes%202.0.pdf) 
- Ferramenta: **[draw.io](https://app.diagrams.net/)**

## Descrição do Projeto

Este projeto, desenvolvido como parte do componente curricular de Padrões de Projetos do curso de Engenharia da Computação do IFPB - Campus Campina Grande, é um software em C++ que simula o funcionamento de um hidrômetro analógico. O objetivo é substituir um hidrômetro real em testes e experimentos, permitindo a visualização contínua do consumo de água e a extração de dados. 

Nesta versão, o simulador gera uma janela de exibição gráfica (pop-up) que é atualizada em tempo real, mostrando a leitura do consumo no formato de um hidrômetro real, com dígitos pretos para metros cúbicos (m³) e vermelhos para frações (litros).

## Funcionalidades Principais

* **Simulação Contínua:** O programa roda de forma ininterrupta até ser encerrado manualmente pelo usuário (Ctrl+C). 
* **Display Gráfico:** Exibe uma janela com a imagem de um hidrômetro, atualizando os números da leitura em tempo real. 
* **Vazão Configurável:** Permite definir a vazão de água (em litros por minuto) através do arquivo `config.txt`. 
* **Simulação de Ar:** Possui a opção de simular uma porcentagem de ar no fluxo, que não é contabilizada no consumo. 
* **Lógica de Odômetro:** O contador de metros cúbicos zera ao atingir seu limite máximo (9999 m³), simulando um odômetro real. 
* **Configuração Externa:** Todos os parâmetros da simulação são carregados a partir de um arquivo `config.txt`, permitindo testes flexíveis sem a necessidade de recompilar.

## Arquitetura

O projeto foi desenvolvido em C++ com uma arquitetura orientada a objetos. As responsabilidades são divididas em classes como `Controlador`, `Hidrometro`, `Medicao`, `Display`, entre outras.

## Tecnologias Utilizadas

* **Linguagem:** C++ (padrão C++17)
* **Compilador:** g++ (MinGW-w64 no Windows)
* **Biblioteca Gráfica:** [CImg Library](https://cimg.eu/) (para a criação da janela e desenho na imagem)
* **IDE:** Visual Studio Code

## Como Configurar e Compilar

Siga os passos abaixo para compilar e executar o projeto.

### 1. Pré-requisitos
* Um compilador C++ **g++ (MinGW-w64)** configurado no PATH do sistema. Recomenda-se a instalação via [MSYS2](https://www.msys2.org/).
* A biblioteca **CImg**: O arquivo `CImg.h` deve estar presente na pasta do projeto.

### 2. Estrutura de Arquivos
Todos os arquivos (`.cpp`, `.h`, `CImg.h`, `config.txt`, `base_hidrometro.bmp`) devem estar na mesma pasta raiz do projeto.

### 3. Compilação
Abra um terminal (PowerShell ou CMD) na pasta raiz do projeto e execute o seguinte comando:
```sh
g++ *.cpp -o simulador.exe -lgdiplus -lgdi32 -luser32 -std=c++17
```
Isso irá compilar todos os arquivos fonte e gerar o executável `simulador.exe`.

## Configuração (`config.txt`)

O comportamento do simulador é controlado pelos seguintes parâmetros no arquivo `config.txt`:

* **VAM**: Vazão de Água por Minuto (0 a 100, mas aceita valores maiores para testes). 
* **PCT_AR**: Percentual de ar no fluxo (0 a 100). 
* **TAXA_IMG**: Intervalo, em m³, para a atualização do display. Ex: `1` para atualizar a cada m³, `0.1` para atualizar a cada 100 litros.
* **CAMINHO**: (Não utilizado na versão com pop-up) Caminho para salvar a imagem. 
* **FORMATO**: (Não utilizado na versão com pop-up) Formato da imagem. 

## Como Executar

Após a compilação, execute o programa pelo terminal:
```sh
./simulador.exe
```
Uma janela pop-up aparecerá com o simulador em funcionamento. Para encerrar a simulação, feche a janela ou pressione **`Ctrl+C`** no terminal.

