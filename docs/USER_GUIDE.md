# Film Neg Invert: Guia do Usuario

Taller Lab: https://tallerlab.com/

## PT-BR

English version bellow.

### Instalacao

Fluxo recomendado:

1. baixe o repositorio;
2. execute `installer.bat` como administrador;
3. reabra o DaVinci Resolve.

O plugin sera instalado em:

`C:\Program Files\Common Files\OFX\Plugins\FilmNegInvert.ofx.bundle\Contents\Win64\FilmNegInvert.ofx`

### Onde encontrar no Resolve

No DaVinci Resolve, procure o efeito em:

`OpenFX > Color > Film Neg Invert`

### Controles atuais

#### Mask Color

Define a cor da base do negativo. E o controle mais importante para neutralizar dominantes.

#### Film Gamma

Controla a curva tonal principal do plugin.

#### Exposure

Aplica ganho global depois da correcao da base e da curva tonal.

#### Input Type

- `Linear (RAW)` para scans lineares
- `Log (Cineon/DPX)` para arquivos realmente em log estilo Cineon/DPX

Se voce usar `Log (Cineon/DPX)` em um arquivo que ja nao esteja nesse tipo de codificacao, a imagem pode ficar superexposta.

#### Film Type

- `Vision3 (Color)` para negativo colorido
- `Double-X (B&W)` para fluxo em preto e branco

`Double-X` tambem forca a saida monocromatica.

#### Red Factor / Green Factor / Blue Factor

Ajuste fino por canal para equilibrar a imagem.

#### Color Invert

Ativa a inversao final. Em muitos casos, esse controle precisa estar ligado para visualizar o positivo.

#### Reset

Restaura os defaults iniciais do plugin.

### Fluxo de uso recomendado

Para um negativo colorido comum:

1. aplique o plugin;
2. deixe `Input Type` correto;
3. escolha `Film Type`;
4. ajuste `Mask Color`;
5. ajuste `Film Gamma`;
6. ligue `Color Invert`;
7. ajuste `Exposure`;
8. refine com `Red`, `Green` e `Blue Factor`.

### Quando usar Linear e quando usar Log

Use `Linear (RAW)` quando:

- o scan ja esta linear;
- o arquivo ja foi convertido para um estado neutro de trabalho;
- voce estiver em duvida.

Use `Log (Cineon/DPX)` quando:

- o material realmente veio em DPX log;
- o scan foi salvo em uma codificacao log no estilo Cineon.

Se o modo `Log` ficar agressivo demais, o mais provavel e que o arquivo nao esteja nesse formato de entrada.

### Problemas comuns

#### A imagem fica estourada

- revise `Input Type`;
- reduza `Exposure`;
- ajuste `Mask Color`;
- teste `Film Gamma` mais alto.

#### A imagem fica com dominante forte

- ajuste `Mask Color`;
- refine com os fatores RGB;
- confirme que o tipo de filme escolhido faz sentido para o material.

#### O plugin nao aparece

- confirme a instalacao em `Program Files`;
- reabra o Resolve;
- confirme que o bundle esta em `FilmNegInvert.ofx.bundle`.

---

## EN

### Installation

Recommended flow:

1. download the repository;
2. run `installer.bat` as administrator;
3. reopen DaVinci Resolve.

The plugin will be installed to:

`C:\Program Files\Common Files\OFX\Plugins\FilmNegInvert.ofx.bundle\Contents\Win64\FilmNegInvert.ofx`

### Where to find it in Resolve

In DaVinci Resolve, look for the effect at:

`OpenFX > Color > Film Neg Invert`

### Current controls

#### Mask Color

Defines the color of the negative film base. This is the most important control for neutralizing casts.

#### Film Gamma

Controls the plugin's main tone curve.

#### Exposure

Applies global gain after base correction and tone shaping.

#### Input Type

- `Linear (RAW)` for linear scans
- `Log (Cineon/DPX)` for files that are actually stored in Cineon/DPX-style log

If you use `Log (Cineon/DPX)` on a file that is not really encoded that way, the image may become severely overexposed.

#### Film Type

- `Vision3 (Color)` for color negative
- `Double-X (B&W)` for black-and-white workflows

`Double-X` also forces monochrome output.

#### Red Factor / Green Factor / Blue Factor

Fine per-channel adjustment for balancing the image.

#### Color Invert

Enables the final inversion. In many cases, this needs to be enabled to view the positive image.

#### Reset

Restores the plugin's startup defaults.

### Recommended workflow

For a typical color negative:

1. apply the plugin;
2. set the correct `Input Type`;
3. choose `Film Type`;
4. adjust `Mask Color`;
5. adjust `Film Gamma`;
6. enable `Color Invert`;
7. adjust `Exposure`;
8. refine with `Red`, `Green`, and `Blue Factor`.

### When to use Linear and when to use Log

Use `Linear (RAW)` when:

- the scan is already linear;
- the file has already been converted into a neutral working state;
- you are unsure.

Use `Log (Cineon/DPX)` when:

- the material actually came in DPX log;
- the scan was stored in a Cineon-style log encoding.

If `Log` feels too aggressive, the most likely reason is that the source file is not actually in that input format.

### Common issues

#### The image blows out

- review `Input Type`;
- reduce `Exposure`;
- adjust `Mask Color`;
- try a higher `Film Gamma`.

#### The image has a strong color cast

- adjust `Mask Color`;
- refine with RGB factors;
- confirm that the selected film type matches the material.

#### The plugin does not appear

- confirm the installation under `Program Files`;
- reopen Resolve;
- confirm that the bundle is named `FilmNegInvert.ofx.bundle`.
