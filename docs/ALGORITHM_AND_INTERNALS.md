# Film Neg Invert: Algoritmo e Funcionamento Interno

Taller Lab: https://tallerlab.com/

## PT-BR

English version bellow.

### Objetivo

O plugin existe para inverter scans de negativo e compensar a base colorida do filme. No estado atual, ele usa um modelo tecnico e pratico, nao uma simulacao fotoquimica completa.

### Pipeline do processamento

Para cada pixel, o plugin faz esta sequencia:

1. interpreta a entrada como `Linear` ou `Log`;
2. remove a cor da base do filme com `Mask Color`;
3. aplica uma curva tonal via `Film Gamma`;
4. aplica ganho global com `Exposure`;
5. aplica ganho por canal com `Red`, `Green` e `Blue Factor`;
6. opcionalmente converte para mono em `Double-X`;
7. opcionalmente inverte com `Color Invert`;
8. limita o resultado para `0..1`.

### Formula central

Para cada canal:

`output = pow(max(0, input * invMask), gammaExponent) * channelFactor * exposure`

Onde:

- `invMask = 1 / maskColor`
- `gammaExponent = 1 / gamma`
- `channelFactor` e o fator individual de cada canal
- `exposure` e o ganho global

### Input Type

#### Linear (RAW)

A entrada segue direto para a etapa de correcao.

#### Log (Cineon/DPX)

Antes da remocao da mascara, o plugin faz uma conversao inspirada em Cineon:

`linear = pow(10, (codeValue - 685) / 300)`

Esse modo so faz sentido quando o scan realmente esta em log no estilo Cineon/DPX. Se o arquivo ja estiver linearizado, em Rec.709, ou em outro pipeline de visualizacao, esse modo tende a estourar com facilidade.

### Mask Color

`Mask Color` representa a cor da base do negativo. O plugin nao subtrai essa cor; ele multiplica cada canal pelo inverso do valor configurado.

Na pratica:

- valor baixo demais tende a explodir brilho;
- valor alto demais tende a abafar a imagem;
- proporcao errada entre os canais gera dominante.

### Film Gamma

`Film Gamma` controla a curva tonal principal do plugin. Internamente, o codigo usa `1 / gamma` como expoente de `pow()`.

Na pratica:

- gamma menor abre a imagem;
- gamma maior comprime a resposta.

### Film Type

`Film Type` hoje nao troca o algoritmo base. Ele aplica presets praticos:

- `Vision3 (Color)` ajusta defaults para negativo colorido;
- `Double-X (B&W)` ajusta defaults mais neutros e forca saida monocromatica.

### Conversao para preto e branco

Quando `Double-X` esta ativo, o plugin calcula luminancia usando:

- `0.2126` para vermelho
- `0.7152` para verde
- `0.0722` para azul

O resultado e copiado para os tres canais.

### Suporte de imagem

O plugin suporta:

- `8-bit unsigned`
- `16-bit unsigned`
- `32-bit float`
- `RGB`
- `RGBA`

O alpha, quando presente, e preservado.

### Estrategia atual de performance

#### Caminho inteiro

Para `8-bit` e `16-bit`, o plugin:

- usa LUTs por canal;
- evita `pow()` no loop quente;
- processa a imagem por ponteiros de linha.

#### Caminho float

Para `float`, o plugin:

- usa LUT com interpolacao linear na faixa principal;
- faz fallback para conta direta acima da faixa da LUT;
- preserva melhor highlights acima de `1.0` antes do clamp final.

### Estado atual do projeto

O plugin atual:

- nao tem GPU;
- nao tem analise automatica de mascara;
- nao faz simulacao fotoquimica completa;
- nao usa estado temporal entre frames.

### Resumo curto

O `Film Neg Invert` e um pipeline parametricamente controlado para:

1. opcionalmente converter log para linear;
2. neutralizar a base colorida do filme;
3. aplicar curva;
4. ajustar ganho;
5. opcionalmente gerar mono;
6. opcionalmente inverter;
7. entregar um resultado pronto para grade adicional.

---

## EN

### Goal

The plugin exists to invert film negative scans and compensate for the colored film base. In its current state, it uses a practical technical model, not a full photochemical simulation.

### Processing pipeline

For each pixel, the plugin applies this sequence:

1. interprets the input as `Linear` or `Log`;
2. removes the film base color using `Mask Color`;
3. applies a tone curve through `Film Gamma`;
4. applies global gain through `Exposure`;
5. applies per-channel gain through `Red`, `Green`, and `Blue Factor`;
6. optionally converts to monochrome in `Double-X`;
7. optionally inverts through `Color Invert`;
8. clamps the result to `0..1`.

### Core formula

For each channel:

`output = pow(max(0, input * invMask), gammaExponent) * channelFactor * exposure`

Where:

- `invMask = 1 / maskColor`
- `gammaExponent = 1 / gamma`
- `channelFactor` is the per-channel factor
- `exposure` is the global gain

### Input Type

#### Linear (RAW)

The input goes directly into the correction stage.

#### Log (Cineon/DPX)

Before mask removal, the plugin performs a Cineon-inspired conversion:

`linear = pow(10, (codeValue - 685) / 300)`

This mode only makes sense when the scan is actually stored in Cineon/DPX-style log. If the file is already linearized, in Rec.709, or in another display-oriented pipeline, this mode tends to blow out quickly.

### Mask Color

`Mask Color` represents the color of the negative film base. The plugin does not subtract it; it multiplies each channel by the inverse of the configured value.

In practice:

- a value that is too low tends to blow out the image;
- a value that is too high tends to mute the image;
- the wrong channel ratio creates color casts.

### Film Gamma

`Film Gamma` controls the plugin's main tone curve. Internally, the code uses `1 / gamma` as the exponent in `pow()`.

In practice:

- lower gamma opens the image;
- higher gamma compresses the response.

### Film Type

`Film Type` does not switch the core algorithm today. It applies practical presets:

- `Vision3 (Color)` sets defaults for color negative;
- `Double-X (B&W)` sets more neutral defaults and forces monochrome output.

### Black-and-white conversion

When `Double-X` is active, the plugin computes luminance using:

- `0.2126` for red
- `0.7152` for green
- `0.0722` for blue

The result is copied to all three channels.

### Image support

The plugin supports:

- `8-bit unsigned`
- `16-bit unsigned`
- `32-bit float`
- `RGB`
- `RGBA`

Alpha, when present, is preserved.

### Current performance strategy

#### Integer path

For `8-bit` and `16-bit`, the plugin:

- uses per-channel LUTs;
- avoids `pow()` in the hot loop;
- processes the image through row pointers.

#### Float path

For `float`, the plugin:

- uses a LUT with linear interpolation over the main range;
- falls back to direct evaluation above the LUT range;
- preserves highlights above `1.0` more faithfully before the final clamp.

### Current project state

The current plugin:

- has no GPU path;
- has no automatic mask analysis;
- does not perform a full photochemical simulation;
- does not use temporal state across frames.

### Short summary

`Film Neg Invert` is a parameter-controlled pipeline that:

1. optionally converts log to linear;
2. neutralizes the colored film base;
3. applies a curve;
4. adjusts gain;
5. optionally generates monochrome output;
6. optionally inverts;
7. delivers an image ready for additional grading.
