# 🌈 Projeto: Sistema de Monitoramento de Luz e Cor com Raspberry Pi Pico W

## 📑 Sumário

- [🎯 Objetivos](#-objetivos)
- [📋 Descrição do Projeto](#-descrição-do-projeto)
- [⚙️ Funcionalidades Implementadas](#️-funcionalidades-implementadas)
- [🛠️ Requisitos do Projeto](#️-requisitos-do-projeto)
- [📂 Estrutura do Código](#-estrutura-do-código)
- [🖥️ Como Compilar](#️-como-compilar)
- [🤝 Contribuições](#-contribuições)
- [📽️ Demonstração em Vídeo](#️-demonstração-em-vídeo)
- [💡 Considerações Finais](#-considerações-finais)

## 🎯 Objetivos

- Desenvolver um sistema de **monitoramento de luminosidade e cores** utilizando a Raspberry Pi Pico W.  
- Apresentar em um **display OLED SSD1306** os valores lidos em tempo real.  
- Representar a intensidade de luz em uma **matriz de LEDs WS2812**, com brilho proporcional ao valor de lux medido.  
- Exibir a cor predominante detectada pelo sensor GY-33 na matriz.  
- Permitir interação com **botões físicos** e indicar modos por meio de **LED RGB**.  

## 📋 Descrição do Projeto

Este projeto utiliza sensores de luz e cor para medir a iluminação ambiente (BH1750) e detectar cores (GY-33). Os dados são exibidos em tempo real em um **display OLED** e representados visualmente em uma **matriz de LEDs WS2812**, onde:

- A **intensidade do brilho** varia de acordo com o nível de lux lido.  
- A **cor exibida** é escolhida de acordo com a componente dominante (R, G ou B) do sensor GY-33.  
- Um **LED RGB discreto** sinaliza modos de operação controlados por botões físicos.  

## ⚙️ Funcionalidades Implementadas

1. **Leitura de luminosidade com o sensor BH1750**  
   - Mede a intensidade de luz em lux.  
   - Normaliza o valor para a faixa de brilho dos LEDs (0–255).  

2. **Leitura de cor com o sensor GY-33**  
   - Detecta valores de vermelho, verde e azul.  
   - Determina a **cor dominante** e a exibe na matriz WS2812.  

3. **Display OLED SSD1306**  
   - Exibe os valores de RGB, Clear e Lux em tempo real.  
   - Apresenta informações de status do sistema.  

4. **Matriz de LEDs WS2812 (5x5)**  
   - Exibe brilho proporcional ao lux.  
   - Mostra a cor predominante detectada pelo sensor GY-33.  

5. **LED RGB discreto**  
   - Controlado por estados definidos pelo botão A.  
   - Alterna entre vermelho, amarelo, verde e azul.  

6. **Botões físicos**  
   - **Botão A (GPIO 5):** alterna o estado do LED RGB.  
   - **Botão B (GPIO 6):** reinicia o dispositivo em modo BOOTSEL.  

## 🛠️ Requisitos do Projeto

- **Raspberry Pi Pico W** com **Pico SDK**.  
- **Sensores:**
  - BH1750 (luxímetro via I2C).  
  - GY-33 (sensor de cor via I2C).  
- **Display:** OLED SSD1306 (I2C).  
- **Saídas visuais:**
  - LED RGB discreto (GPIOs 11, 12, 13).  
  - Matriz WS2812 (GPIO 7).  
- **Botões:** A (GPIO 5) e B (GPIO 6).  
- **Bibliotecas utilizadas:**
  - `pico/stdlib.h`  
  - `hardware/i2c.h`  
  - Drivers `bh1750_light_sensor.h`, `ssd1306.h`, `font.h`  
  - Código PIO para WS2812 (`ws2812.pio.h`).  

## 📂 Estrutura do Código

```
├── lux_cor.c # Código principal da aplicação
├── CMakeLists.txt # Configuração de compilação
└── lib/
├── bh1750_light_sensor.h # Driver do sensor BH1750
├── ssd1306.c # Driver do display OLED
├── ssd1306.h # Header do display
├── font.h # Fonte para exibição no display
├── ws2812.pio.h # Código PIO para matriz WS2812
└── matriz.h # Funções de controle da matriz
```
## 🖥️ Como Compilar

1. Clone o repositório:
   ```bash
   git clone https://github.com/JPSRaccolto/lux_e_cor.git
   ```
2. Entre no diretório do projeto:
```bash
  cd lux_e_cor/lux_cor
```
3.  Compile o projeto utilizando o VS Code com a extensão Raspberry Pi Pico e as tarefas de compilação configuradas.
   
## 🖥️ Método alternativo:

1.  Baixe o repositório como um arquivo ZIP.
2.  Extraia para uma pasta de fácil acesso.
3.  Utilize a extensão **Raspberry Pi Pico** dentro do VS Code para importar o projeto.
4.  Aguarde até que a configuração inicial crie o diretório `build`.
5.  Utilize o ícone de **Build** na barra de status para compilar o projeto.
6.  Coloque a Raspberry Pi Pico em modo **BOOTSEL** e utilize o ícone de **Run** para carregar o programa na placa.
7.  Interaja com os botões e acesse o IP do dispositivo (exibido no terminal serial) para explorar todas as funcionalidades.

## 🧑‍💻 Autores
```
João Pedro Soares Raccolto
Nivaldo Rodrigues da Silva Júnior
Rian de Sena Mendes
Samuel Guedes Canário
```
## 🤝 Contribuições

Este projeto foi desenvolvido por:
```
João Pedro Soares Raccolto
Nivaldo Rodrigues da Silva Júnior
Rian de Sena Mendes
Samuel Guedes Canário
```
Contribuições são bem-vindas! Siga os passos abaixo para contribuir:

1.  Faça um Fork deste repositório.
2.  Crie uma nova branch:
    ```bash
    git checkout -b minha-feature
    ```
3.  Faça suas modificações e commit:
    ```bash
    git commit -m 'Minha nova feature'
    ```
4.  Envie suas alterações:
    ```bash
    git push origin minha-feature
    ```
5.  Abra um Pull Request.

## 📽️ Demonstração em Vídeo

- Um vídeo de demonstração do projeto pode ser disponibilizado aqui: [Link para o vídeo](https://drive.google.com/file/d/1n5OwWFpV1t55j5IWqhUlCTDnHYD49xRZ/view?usp=drivesdk)

## 💡 Considerações Finais
Este projeto demonstra como a Raspberry Pi Pico W pode integrar sensores de luz e cor com saídas visuais (OLED, matriz WS2812, LED RGB, Buzzer) e controles físicos.
A solução é útil para aplicações didáticas e protótipos de sistemas de monitoramento ambiental e de iluminação inteligente.
