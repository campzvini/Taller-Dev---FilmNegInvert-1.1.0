# **Film Neg Invert \- Plugin OpenFX para DaVinci Resolve**

Taller Dev \- 2026

O **Film Neg Invert** é um plugin OpenFX (OFX) de alta precisão desenvolvido para o **DaVinci Resolve (Windows)**, projetado especificamente para a inversão e reconstrução radiométrica de negativos de cinema digitalizados. Diferente de ferramentas de inversão genéricas, este plugin implementa um pipeline científico que respeita as curvas sensitométricas e a física das emulsões cinematográficas, com foco nas famílias **Kodak Vision3** e **Eastman Double-X**.

## ---

**Principais Recursos**

* **Pipeline Científico**: Subtração da máscara laranja (D-min) e linearização baseada em dados técnicos oficiais da Kodak.  
* **Análise de Máscara**: Algoritmo para detecção e neutralização da base de acetato do filme.  
* **Versatilidade de Entrada**: Suporte para fluxos de trabalho Lineares (Scans RAW) e Logarítmicos (Cineon/DPX).  
* **Presets de Emulsão**: Otimizações para Kodak Vision3 50D, 250D, 200T, 500T e Eastman Double-X 5222\.  
* **Performance**: Código C++ otimizado para processamento de sequências em resoluções 4K e 8K.

## ---

**Instalacao**

### **Requisitos de Sistema**

* **Sistema Operacional**: Windows 10/11 (64-bit).  
* **Software Host**: DaVinci Resolve 17 ou superior.  
* **Hardware**: Processador com suporte ao conjunto de instruções SSE4.2.

### **Instalacao Automatica (Recomendada)**

1. Baixe o arquivo comprimido da versao estavel em Releases.  
2. Extraia o conteudo para a pasta raiz do projeto.  
3. Clique com o botao direito no arquivo install.bat e selecione **Executar como administrador**.  
4. O script criara automaticamente a estrutura de diretorios e instalara o plugin no local correto.

### **Instalacao Manual**

1. Baixe o pacote do plugin.  
2. Mova a pasta do bundle para o diretorio padrao de plugins OFX do Windows: C:\\Program Files\\Common Files\\OFX\\Plugins\\FilmNegInvert.ofx.bundle\\Contents\\Win64\\  
3. Certifique-se de que o arquivo FilmNegInvert.ofx esteja dentro da subpasta Win64.

### **Pos-Instalacao**

1. Reinicie o DaVinci Resolve.  
2. O plugin estara disponivel na biblioteca de efeitos em **Color \> OpenFX**.

## ---

**Desenvolvimento e Build**

O projeto utiliza o padrão C++17 e o sistema CMake 3.18+ para garantir compatibilidade binária com o DaVinci Resolve.

\# Configuracao do ambiente de build  
mkdir build && cd build  
cmake .. \-DCMAKE\_BUILD\_TYPE=Release \-DOFX\_ROOT=C:/dev/openfx  
cmake \--build . \--config Release

---

**Referencias e Agradecimentos**   
Este projeto foi inspirado e utilizou conceitos fundamentais do projeto kaliscope, desenvolvido por Eloi DU BOIS (licença GNU GPL v3) disponibilizado pelo filmlabs.org. Reconhecemos o valor técnico do kaliscope na compreensão de remoção de máscaras coloridas e conversões RGB/YUV para preservação fílmica.

# ---

**Film Neg Invert \- OpenFX Plugin for DaVinci Resolve**

**Film Neg Invert** is a high-precision OpenFX (OFX) plugin for **DaVinci Resolve (Windows)**, purpose-built for the inversion and radiometric reconstruction of scanned motion picture film negatives. This plugin implements a scientific pipeline that respects sensitometric curves and the physics of cinema emulsions, specifically targeting the **Kodak Vision3** and **Eastman Double-X** families.

## ---

**Key Features**

* **Scientific Pipeline**: D-min (orange mask) subtraction and linearization based on official Kodak technical data.  
* **Mask Analysis**: Advanced algorithm for detection and neutralization of the film acetate base.  
* **Input Versatility**: Native support for both Linear (RAW Scans) and Logarithmic (Cineon/DPX) workflows.  
* **Dedicated Presets**: Specific optimizations for Vision3 (Color) and Double-X (B\&W) film stocks.  
* **Performance**: Optimized C++ codebase capable of handling 4K and 8K image sequences.

## ---

**Installation**

### **System Requirements**

* **OS**: Windows 10/11 (64-bit).  
* **Host Software**: DaVinci Resolve 17 or higher.  
* **Hardware**: CPU with SSE4.2 instruction set support.

### **Automatic Installation (Recommended)**

1. Download the latest stable package from the Releases tab.  
2. Extract the files to the project root folder.  
3. Right-click the install.bat file and select **Run as administrator**.  
4. The script will automatically generate the required directory structure and install the plugin to the correct path.

### **Manual Installation**

1. Download the plugin package.  
2. Move the bundle folder to the system's standard OFX plugin directory: C:\\Program Files\\Common Files\\OFX\\Plugins\\FilmNegInvert.ofx.bundle\\Contents\\Win64\\  
3. Ensure the FilmNegInvert.ofx file is correctly placed inside the Win64 subfolder.

### **Post-Installation**

1. Restart DaVinci Resolve.  
2. The plugin will be located in the effects library under **Color \> OpenFX**.

## ---

**Development and Build**

The project leverages C++17 and CMake 3.18+ to ensure stability and ABI compatibility with the host application.

\# Build configuration  
mkdir build && cd build  
cmake .. \-DCMAKE\_BUILD\_TYPE=Release \-DOFX\_ROOT=C:/dev/openfx  
cmake \--build . \--config Release

## ---

**Acknowledgments**

This project was inspired by and utilized fundamental concepts from the kaliscope project, developed by Eloi DU BOIS (GNU GPL v3 license) made available by filmlabs.org. We recognize the technical value of kaliscope in the understanding of colored mask removal and RGB/YUV conversions for film preservation.

---

**VAI CORINTHIANS\!\!** ⚫⚪