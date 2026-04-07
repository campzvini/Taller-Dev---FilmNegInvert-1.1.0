# Film Neg Invert

OpenFX plugin for DaVinci Resolve focused on practical film negative inversion on Windows.

Taller Lab: https://tallerlab.com/

## PT-BR

English version bellow.

### O que e

`Film Neg Invert` e um plugin OFX para DaVinci Resolve voltado para inversao tecnica de negativos digitalizados. A proposta do projeto nao e simular um laboratorio fotoquimico completo. O foco e oferecer um pipeline pratico, reproduzivel e rapido para:

- neutralizar a base colorida do negativo;
- ajustar a resposta tonal;
- inverter para positivo;
- trabalhar em lotes grandes de frames.

### Agradecimentos e referencia de pesquisa

Este projeto foi iniciado durante o estudo do projeto `kaliscope`, de Eloi du Bois:

https://github.com/edubois/kaliscope

O `kaliscope` foi uma referencia importante de pesquisa e contexto tecnico, servindo de grande auxilio para o desenvolvimento de processos fotoquimicos no Brasil. 

### Estado real da versao 1.1.0

- Windows apenas.
- Plugin OpenFX para DaVinci Resolve.
- Suporte a `RGB` e `RGBA`.
- Suporte a `8-bit`, `16-bit` e `32-bit float`.
- Render em CPU com multithreading por faixas de linhas.
- Caminho inteiro (`8/16-bit`) otimizado com LUT.
- Caminho `float` com LUT interpolada na faixa principal e fallback matematico acima da faixa da LUT.
- Entrada `Linear (RAW)` e `Log (Cineon/DPX)`.
- Presets praticos para `Vision3 (Color)` e `Double-X (B&W)`.
- `Double-X` forca saida monocromatica.
- Sem processamento GPU no estado atual.
- Sem analise automatica de mascara no estado atual.
- Projeto em fase de testes enquanto avaliamos uma futura implementacao de GPU.

### Nota de desenvolvimento

Projeto desenvolvido com auxilio de Codex, GLM-4.7 e DeepSeek, com direcao, integracao e validacao final do Taller Lab.

### Instalacao rapida

O repositorio foi preparado para distribuicao direta. O fluxo esperado e:

1. baixar o repositorio do GitHub;
2. extrair ou clonar;
3. executar `installer.bat`;
4. aceitar privelegio de admin;
5. abrir o DaVinci Resolve.

O instalador usa primeiro o binario publico versionado em:

`dist\FilmNegInvert.ofx`

Depois ele instala o plugin em:

`C:\Program Files\Common Files\OFX\Plugins\FilmNegInvert.ofx.bundle\Contents\Win64\FilmNegInvert.ofx`

O icone do plugin fica em:

`C:\Program Files\Common Files\OFX\Plugins\FilmNegInvert.ofx.bundle\Contents\Resources\com.tallerneg.FilmNegInvert.png`

### Requisitos

- Windows 10 ou 11 64-bit
- DaVinci Resolve com suporte a OpenFX
- permissao de administrador para escrever em `Program Files`

O binario distribuido usa runtime estatico do MSVC. Na pratica, isso evita dependencia separada do Visual C++ Redistributable na maquina de destino.

### Controles atuais

- `Mask Color`
- `Film Gamma`
- `Exposure`
- `Input Type`
- `Film Type`
- `Red Factor`
- `Green Factor`
- `Blue Factor`
- `Color Invert`
- `Reset`

### Documentacao publica

- [docs/USER_GUIDE.md](docs/USER_GUIDE.md)
- [docs/ALGORITHM_AND_INTERNALS.md](docs/ALGORITHM_AND_INTERNALS.md)
- [CONTRIBUTING.md](CONTRIBUTING.md)

### Estrutura publica do projeto

```text
/
  README.md
  CONTRIBUTING.md
  LICENSE
  installer.bat
  CMakeLists.txt
  CMakeSettings.json
  TallerNegInvert.def
  dist/
    FilmNegInvert.ofx
  docs/
    USER_GUIDE.md
    ALGORITHM_AND_INTERNALS.md
  external/
    openfx/
  include/
    FilmNegInvertPlugin.h
    FilmNegInvertPluginFactory.h
  Resources/
    com.tallerneg.FilmNegInvert.png
  src/
    FilmNegInvertPlugin.cpp
    FilmNegInvertPluginFactory.cpp
    mainEntry.cpp
```

### Build para desenvolvimento

Build principal:

```powershell
cmake -S . -B out/build/x64-Release -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build out/build/x64-Release --config RelWithDebInfo
```

Se faltar ambiente do MSVC:

```powershell
cmd /c "call \"C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat\" -arch=x64 && cmake --build out\build\x64-Release --config RelWithDebInfo"
```

Se voce gerar uma nova versao para distribuicao publica, atualize:

`dist\FilmNegInvert.ofx`

### Licenca

Este repositorio inclui o codigo do plugin e uma copia vendorizada do OpenFX support framework em `external/openfx`. Antes de publicar forks, releases ou pacotes derivados, revise as licencas dos componentes incluidos.

---

## EN

### What it is

`Film Neg Invert` is an OFX plugin for DaVinci Resolve focused on practical technical inversion of scanned film negatives. The goal is not to simulate a full photochemical lab. The project is built to provide a practical, reproducible, and fast pipeline for:

- neutralizing the film base color;
- shaping tonal response;
- inverting to a positive image;
- processing large frame batches.

### Acknowledgements and research reference

This project started while studying `kaliscope`, by Eloi du Bois:

https://github.com/edubois/kaliscope

`kaliscope` was an important research and technical reference, providing major support for the development of photochemical processes in Brazil.

### Real state of version 1.1.0

- Windows only.
- OpenFX plugin for DaVinci Resolve.
- Supports `RGB` and `RGBA`.
- Supports `8-bit`, `16-bit`, and `32-bit float`.
- CPU rendering with row-sliced multithreading.
- Optimized integer path (`8/16-bit`) using LUTs.
- `float` path with interpolated LUT over the main range and direct math fallback above the LUT range.
- `Linear (RAW)` and `Log (Cineon/DPX)` input modes.
- Practical presets for `Vision3 (Color)` and `Double-X (B&W)`.
- `Double-X` forces monochrome output.
- No GPU processing in the current version.
- No automatic mask analysis in the current version.
- The project is currently in testing while we evaluate a future GPU implementation.

### Development note

This project was developed with assistance from Codex, GLM-4.7, and DeepSeek, with final direction, integration, and validation by Taller Lab.

### Quick install

The repository is prepared for direct distribution. The intended flow is:

1. download the repository from GitHub;
2. extract or clone it;
3. run `installer.bat`;
4. accept the elevation prompt;
5. open DaVinci Resolve.

The installer first uses the public versioned binary stored at:

`dist\FilmNegInvert.ofx`

Then it installs the plugin to:

`C:\Program Files\Common Files\OFX\Plugins\FilmNegInvert.ofx.bundle\Contents\Win64\FilmNegInvert.ofx`

The plugin icon is installed to:

`C:\Program Files\Common Files\OFX\Plugins\FilmNegInvert.ofx.bundle\Contents\Resources\com.tallerneg.FilmNegInvert.png`

### Requirements

- Windows 10 or 11 64-bit
- DaVinci Resolve with OpenFX support
- administrator permissions to write into `Program Files`

The distributed binary uses the static MSVC runtime. In practice, this avoids a separate Visual C++ Redistributable dependency on the target machine.

### Current controls

- `Mask Color`
- `Film Gamma`
- `Exposure`
- `Input Type`
- `Film Type`
- `Red Factor`
- `Green Factor`
- `Blue Factor`
- `Color Invert`
- `Reset`

### Public documentation

- [docs/USER_GUIDE.md](docs/USER_GUIDE.md)
- [docs/ALGORITHM_AND_INTERNALS.md](docs/ALGORITHM_AND_INTERNALS.md)
- [CONTRIBUTING.md](CONTRIBUTING.md)

### Public project layout

```text
/
  README.md
  CONTRIBUTING.md
  LICENSE
  installer.bat
  CMakeLists.txt
  CMakeSettings.json
  TallerNegInvert.def
  dist/
    FilmNegInvert.ofx
  docs/
    USER_GUIDE.md
    ALGORITHM_AND_INTERNALS.md
  external/
    openfx/
  include/
    FilmNegInvertPlugin.h
    FilmNegInvertPluginFactory.h
  Resources/
    com.tallerneg.FilmNegInvert.png
  src/
    FilmNegInvertPlugin.cpp
    FilmNegInvertPluginFactory.cpp
    mainEntry.cpp
```

### Development build

Main build:

```powershell
cmake -S . -B out/build/x64-Release -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build out/build/x64-Release --config RelWithDebInfo
```

If the MSVC environment is missing:

```powershell
cmd /c "call \"C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat\" -arch=x64 && cmake --build out\build\x64-Release --config RelWithDebInfo"
```

If you generate a new public distributable build, refresh:

`dist\FilmNegInvert.ofx`

### License

This repository includes the plugin code and a vendored copy of the OpenFX support framework under `external/openfx`. Before publishing forks, releases, or derived packages, review the licenses of the included components.


VAI CORINTHIANS!!
