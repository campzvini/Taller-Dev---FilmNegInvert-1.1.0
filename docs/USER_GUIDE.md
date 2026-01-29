# **Film Neg Invert - Guia do Usuario**

Taller Dev - 2026

Este guia fornece instrucoes detalhadas para o uso do plugin **Film Neg Invert** no DaVinci Resolve. O plugin foi projetado para realizar a inversao de negativos de cinema com precisao matematica, especificamente para as familias **Kodak Vision3** e **Eastman Double-X**.

## ---

**Visao Geral**

O **Film Neg Invert** e uma ferramenta profissional para coloristas e restauradores de filmes que trabalham com scans de negativos de cinema. O plugin implementa um pipeline cientifico de inversao que respeita a fisica da pelicula fotoquimica.

* **Analise Automatica**: Detecta automaticamente a cor da mascara do filme.
* **Multiplos Formatos**: Suporta Linear (RAW) e Log (Cineon/DPX).
* **Presets de Filme**: Vision3 (cor) e Double-X (P&B).
* **Controle Fino**: Ajuste independente por canal RGB.
* **Performance**: Processamento otimizado para 4K/8K.

## ---

**Instalacao**

### **Requisitos de Sistema**

* **Sistema Operacional**: Windows 10/11 (64-bit).
* **Software**: DaVinci Resolve 17 ou superior.
* **Hardware**: Processador com suporte a SSE4.2.

### **Instalacao Automatica (Recomendada)**

1. Baixe o arquivo FilmNegInvert-1.0.0-Win64.zip.
2. Extraia o conteudo.
3. Execute INSTALL.bat como **Administrador**.
4. Reinicie o DaVinci Resolve.

### **Instalacao Manual**

1. Localize o diretorio de plugins OFX do seu sistema:
   C:\\Program Files\\Common Files\\OFX\\Plugins\\

2. Crie a estrutura de diretorios:
   FilmNegInvert.ofx.bundle/
   └── Contents/
       └── Win64/

3. Copie o arquivo FilmNegInvert.ofx para:
   C:\\Program Files\\Common Files\\OFX\\Plugins\\
   FilmNegInvert.ofx.bundle\\Contents\\Win64\\

4. Reinicie o DaVinci Resolve.

### **Verificacao da Instalacao**

Apos a instalacao, verifique se o plugin aparece no DaVinci Resolve:

1. Abra o DaVinci Resolve.
2. Va para a pagina **Color** (Cor).
3. No Node Editor, clique com o botao direito.
4. Navegue ate: **OpenFX > Color > Film Neg Invert**.

## ---

**Uso Basico**

### **Adicionando o Plugin**

1. Na pagina **Color**, adicione um novo node.
2. Clique com o botao direito no node.
3. Selecione **OpenFX > Color > Film Neg Invert**.

### **Interface do Plugin**

A interface do plugin exibe os seguintes controles na aba **Main**:

```
┌─────────────────────────────────────┐
│  Film Neg Invert                    │
├─────────────────────────────────────┤
│  Algorithm: [RGB Reduction ▼]       │
│  Mask Color: [Picker] R: 0.8        │
│              G: 0.5  B: 0.3         │
│  Film Gamma: 0.6                    │
│  Exposure: 1.0                      │
│  Input Type: [Linear ▼]             │
│  Film Type: [Vision3 ▼]             │
│  Red Factor: 1.0                    │
│  Green Factor: 1.0                  │
│  Blue Factor: 1.0                   │
│  [ ] Color Invert                   │
│  [Analyze Mask]  [Help]             │
└─────────────────────────────────────┘
```

## ---

**Parametros**

### **Algorithm (Algoritmo)**

* **Tipo**: Lista de opcoes.
* **Opcoes**: RGB Reduction.
* **Descricao**: Algoritmo de remocao da mascara colorida.

### **Mask Color (Cor da Mascara)**

* **Tipo**: Color Picker RGB.
* **Padrao**: R: 0.8, G: 0.5, B: 0.3 (laranja tipico).
* **Range**: 0.0 - 1.0 para cada canal.
* **Descricao**: Cor base do filme (mascara laranja). Use o picker para amostrar a area entre as perfuracoes do filme.

### **Film Gamma (Gama do Filme)**

* **Tipo**: Valor decimal.
* **Padrao**: 0.6.
* **Range**: 0.1 - 2.0.
* **Display Range**: 0.4 - 1.0.
* **Descricao**: Curva de resposta do filme. Valores tipicos:
  * **Vision3**: 0.6.
  * **Double-X**: 0.5.

### **Exposure (Exposicao)**

* **Tipo**: Valor decimal.
* **Padrao**: 1.0.
* **Range**: 0.0 - 10.0.
* **Display Range**: 0.0 - 5.0.
* **Descricao**: Ajuste de exposicao linear pos-inversao. Valores > 1.0 aumentam a exposicao.

### **Input Type (Tipo de Entrada)**

* **Tipo**: Lista de opcoes.
* **Opcoes**: Linear (RAW), Log (Cineon/DPX).
* **Padrao**: Linear (RAW).
* **Descricao**: Formato da imagem de entrada. Selecione conforme o tipo de scan:
  * **Linear**: Scans RAW diretos.
  * **Log**: Arquivos Cineon, DPX log, ou scans logaritmicos.

### **Film Type (Tipo de Filme)**

* **Tipo**: Lista de opcoes.
* **Opcoes**: Vision3 (Color), Double-X (B&W).
* **Padrao**: Vision3 (Color).
* **Descricao**: Preset do tipo de pelicula:
  * **Vision3**: Pelicula colorida Kodak Vision3 (50D, 250D, 200T, 500T).
  * **Double-X**: Pelicula preto e branco Eastman Double-X.

### **Red/Green/Blue Factor (Fator R/G/B)**

* **Tipo**: Valor decimal.
* **Padrao**: 1.0.
* **Range**: 0.0 - 300.0.
* **Display Range**: 0.0 - 10.0.
* **Descricao**: Fatores de contraste independentes para cada canal. Use para correcoes finas de balanco de cor.

### **Color Invert (Inversao de Cor)**

* **Tipo**: Checkbox.
* **Padrao**: Desmarcado.
* **Descricao**: Inverte as cores da imagem apos o processamento. **Deve estar marcado** para visualizar a imagem positiva.

### **Analyze Mask (Analisar Mascara)**

* **Tipo**: Botao.
* **Descricao**: Analisa a imagem atual e detecta automaticamente a cor da mascara. Clique em uma area do filme que represente a base (entre as perfuracoes).

### **Help (Ajuda)**

* **Tipo**: Botao.
* **Descricao**: Exibe informacoes de ajuda sobre o uso do plugin.

## ---

**Fluxo de Trabalho Recomendado**

### **Para Negativos Coloridos (Vision3)**

1. **Carregue o scan do negativo** no DaVinci Resolve.
2. **Adicione o plugin** Film Neg Invert a um node.
3. **Configure o Input Type**:
   * Selecione "Linear" para scans RAW.
   * Selecione "Log" para arquivos Cineon/DPX.
4. **Selecione o Film Type**: "Vision3 (Color)".
5. **Detecte a mascara**:
   * Clique em **Analyze Mask** ou
   * Use o color picker para selecionar a area entre as perfuracoes.
6. **Ajuste o Film Gamma** se necessario (padrao 0.6 funciona bem para Vision3).
7. **Ative Color Invert** para ver a imagem positiva.
8. **Ajuste a Exposure** para o brilho desejado.
9. **Fine-tuning**: Use os fatores R/G/B para ajustes finos de cor.

### **Para Negativos Preto e Branco (Double-X)**

1. **Carregue o scan do negativo**.
2. **Adicione o plugin** Film Neg Invert.
3. **Configure o Input Type** conforme necessario.
4. **Selecione o Film Type**: "Double-X (B&W)".
5. **Detecte a mascara** usando **Analyze Mask**.
6. **Ajuste o Film Gamma** (tente 0.5 para Double-X).
7. **Ative Color Invert**.
8. **Ajuste a Exposure** conforme necessario.

## ---

**Exemplos de Uso**

### **Exemplo 1: Scan RAW de Vision3 500T**

```
Input Type: Linear (RAW)
Film Type: Vision3 (Color)
Mask Color: [Use Analyze Mask]
Film Gamma: 0.6
Exposure: 1.2
Color Invert: [X] Ativado
```

### **Exemplo 2: DPX Log de Double-X**

```
Input Type: Log (Cineon/DPX)
Film Type: Double-X (B&W)
Mask Color: R: 0.75, G: 0.45, B: 0.25
Film Gamma: 0.5
Exposure: 1.0
Color Invert: [X] Ativado
```

### **Exemplo 3: Correcao de Cor com Fatores RGB**

Se a imagem resultante estiver com dominancia azulada:

```
Red Factor: 1.2
Green Factor: 1.0
Blue Factor: 0.9
```

## ---

**Troubleshooting**

### **Problema: Plugin nao aparece no DaVinci Resolve**

**Causas possiveis:**
* Instalacao incorreta.
* Permissoes insuficientes.
* Resolve nao reiniciado.

**Solucoes:**
1. Verifique se o arquivo esta em:
   C:\\Program Files\\Common Files\\OFX\\Plugins\\
   FilmNegInvert.ofx.bundle\\Contents\\Win64\\FilmNegInvert.ofx
2. Execute o instalador como Administrador.
3. Reinicie o DaVinci Resolve.
4. Verifique o console do Resolve por mensagens de erro.

### **Problema: Imagem fica completamente branca ou preta**

**Causas possiveis:**
* Mask Color incorreta.
* Exposure muito alto ou muito baixo.
* Color Invert nao ativado.

**Solucoes:**
1. Clique em **Analyze Mask** para detectar a cor correta.
2. Reduza o valor de Exposure (tente 1.0).
3. Verifique se **Color Invert** esta ativado.

### **Problema: Cores distorcidas ou irreais**

**Causas possiveis:**
* Mask Color incorreta.
* Input Type incorreto.
* Film Gamma inadequado.

**Solucoes:**
1. Verifique se o Input Type corresponde ao formato do scan.
2. Reajuste a Mask Color usando o picker.
3. Experimente diferentes valores de Film Gamma (0.5 - 0.7).

### **Problema: Performance lenta**

**Causas possiveis:**
* Resolucao muito alta.
* Sistema sobrecarregado.

**Solucoes:**
1. Use proxies para edicao.
2. Reduza a resolucao do timeline durante o ajuste.
3. Aplique o plugin apenas nos clips finais.

## ---

**Dicas e Boas Praticas**

### **1. Escolha da Area para Analise**

Ao usar **Analyze Mask**, selecione uma area que represente a base do filme:
* **Area ideal**: Entre as perfuracoes do filme.
* **Evite**: Areas de imagem ou bordas escuras.
* **Dica**: A cor da mascara tipica e laranja avermelhada.

### **2. Ordem de Ajustes Recomendada**

1. Defina Input Type e Film Type.
2. Configure Mask Color.
3. Ajuste Film Gamma.
4. Ative Color Invert.
5. Ajuste Exposure.
6. Fine-tuning com fatores RGB.

### **3. Valores de Referencia**

| Tipo de Filme | Input Type | Film Gamma | Mask Color tipica |
|---------------|------------|------------|-------------------|
| Vision3 50D   | Linear    | 0.6        | R: 0.75-0.85      |
| Vision3 250D  | Linear    | 0.6        | R: 0.75-0.85      |
| Vision3 200T  | Linear    | 0.6        | R: 0.75-0.85      |
| Vision3 500T  | Linear    | 0.6        | R: 0.75-0.85      |
| Double-X      | Linear    | 0.5        | R: 0.70-0.80      |

### **4. Salvando Presets**

Para salvar configuracoes frequentemente usadas:
1. Ajuste os parametros desejados.
2. Clique com o botao direito no node.
3. Selecione **Save Node Preset**.
4. De um nome descritivo (ex: "Vision3 500T RAW").

### **5. Integracao com o Workflow do Resolve**

O Film Neg Invert funciona melhor como **primeiro node** na cadeia de cor:

```
[Source] → [Film Neg Invert] → [Color Correction] → [Output]
```

Isso permite que voce faca a correcao de cor tradicional sobre a imagem ja invertida.

## ---

**Suporte**

Para reportar problemas ou sugerir melhorias:

* **Documentacao tecnica**: Veja `docs/PROJECT_OVERVIEW.md`.
* **Changelog**: Veja `CHANGELOG.md`.

---

# **Film Neg Invert - User Guide**

Taller Dev - 2026

This guide provides detailed instructions for using the **Film Neg Invert** plugin in DaVinci Resolve. The plugin is designed to perform film negative inversion with mathematical precision, specifically for the **Kodak Vision3** and **Eastman Double-X** families.

## ---

**Overview**

**Film Neg Invert** is a professional tool for colorists and film restorers working with film negative scans. The plugin implements a scientific inversion pipeline that respects the physics of photochemical film.

* **Automatic Analysis**: Automatically detects film mask color.
* **Multiple Formats**: Supports Linear (RAW) and Log (Cineon/DPX).
* **Film Presets**: Vision3 (color) and Double-X (B&W).
* **Fine Control**: Independent RGB channel adjustment.
* **Performance**: Optimized processing for 4K/8K.

## ---

**Installation**

### **System Requirements**

* **Operating System**: Windows 10/11 (64-bit).
* **Software**: DaVinci Resolve 17 or higher.
* **Hardware**: Processor with SSE4.2 support.

### **Automatic Installation (Recommended)**

1. Download the file FilmNegInvert-1.0.0-Win64.zip.
2. Extract the contents.
3. Run INSTALL.bat as **Administrator**.
4. Restart DaVinci Resolve.

### **Manual Installation**

1. Locate the OFX plugins directory on your system:
   C:\\Program Files\\Common Files\\OFX\\Plugins\\

2. Create the directory structure:
   FilmNegInvert.ofx.bundle/
   └── Contents/
       └── Win64/

3. Copy the file FilmNegInvert.ofx to:
   C:\\Program Files\\Common Files\\OFX\\Plugins\\
   FilmNegInvert.ofx.bundle\\Contents\\Win64\\

4. Restart DaVinci Resolve.

### **Installation Verification**

After installation, verify that the plugin appears in DaVinci Resolve:

1. Open DaVinci Resolve.
2. Go to the **Color** page.
3. Right-click in the Node Editor.
4. Navigate to: **OpenFX > Color > Film Neg Invert**.

## ---

**Basic Usage**

### **Adding the Plugin**

1. On the **Color** page, add a new node.
2. Right-click on the node.
3. Select **OpenFX > Color > Film Neg Invert**.

### **Plugin Interface**

The plugin interface displays the following controls in the **Main** tab:

```
┌─────────────────────────────────────┐
│  Film Neg Invert                    │
├─────────────────────────────────────┤
│  Algorithm: [RGB Reduction ▼]       │
│  Mask Color: [Picker] R: 0.8        │
│              G: 0.5  B: 0.3         │
│  Film Gamma: 0.6                    │
│  Exposure: 1.0                      │
│  Input Type: [Linear ▼]             │
│  Film Type: [Vision3 ▼]             │
│  Red Factor: 1.0                    │
│  Green Factor: 1.0                  │
│  Blue Factor: 1.0                   │
│  [ ] Color Invert                   │
│  [Analyze Mask]  [Help]             │
└─────────────────────────────────────┘
```

## ---

**Parameters**

### **Algorithm**

* **Type**: Option list.
* **Options**: RGB Reduction.
* **Description**: Colored mask removal algorithm.

### **Mask Color**

* **Type**: RGB Color Picker.
* **Default**: R: 0.8, G: 0.5, B: 0.3 (typical orange).
* **Range**: 0.0 - 1.0 for each channel.
* **Description**: Film base color (orange mask). Use the picker to sample the area between film perforations.

### **Film Gamma**

* **Type**: Decimal value.
* **Default**: 0.6.
* **Range**: 0.1 - 2.0.
* **Display Range**: 0.4 - 1.0.
* **Description**: Film response curve. Typical values:
  * **Vision3**: 0.6.
  * **Double-X**: 0.5.

### **Exposure**

* **Type**: Decimal value.
* **Default**: 1.0.
* **Range**: 0.0 - 10.0.
* **Display Range**: 0.0 - 5.0.
* **Description**: Linear exposure adjustment post-inversion. Values > 1.0 increase exposure.

### **Input Type**

* **Type**: Option list.
* **Options**: Linear (RAW), Log (Cineon/DPX).
* **Default**: Linear (RAW).
* **Description**: Input image format. Select according to scan type:
  * **Linear**: Direct RAW scans.
  * **Log**: Cineon, DPX log, or logarithmic scans.

### **Film Type**

* **Type**: Option list.
* **Options**: Vision3 (Color), Double-X (B&W).
* **Default**: Vision3 (Color).
* **Description**: Film stock preset:
  * **Vision3**: Kodak Vision3 color film (50D, 250D, 200T, 500T).
  * **Double-X**: Eastman Double-X black and white film.

### **Red/Green/Blue Factor**

* **Type**: Decimal value.
* **Default**: 1.0.
* **Range**: 0.0 - 300.0.
* **Display Range**: 0.0 - 10.0.
* **Description**: Independent contrast factors for each channel. Use for fine color balance adjustments.

### **Color Invert**

* **Type**: Checkbox.
* **Default**: Unchecked.
* **Description**: Inverts image colors after processing. **Must be checked** to view the positive image.

### **Analyze Mask**

* **Type**: Button.
* **Description**: Analyzes the current image and automatically detects the mask color. Click on an area of the film that represents the base (between perforations).

### **Help**

* **Type**: Button.
* **Description**: Displays help information about plugin usage.

## ---

**Recommended Workflow**

### **For Color Negatives (Vision3)**

1. **Load the negative scan** into DaVinci Resolve.
2. **Add the Film Neg Invert plugin** to a node.
3. **Configure Input Type**:
   * Select "Linear" for RAW scans.
   * Select "Log" for Cineon/DPX files.
4. **Select Film Type**: "Vision3 (Color)".
5. **Detect the mask**:
   * Click **Analyze Mask** or
   * Use the color picker to select the area between perforations.
6. **Adjust Film Gamma** if necessary (default 0.6 works well for Vision3).
7. **Enable Color Invert** to see the positive image.
8. **Adjust Exposure** for desired brightness.
9. **Fine-tuning**: Use R/G/B factors for fine color adjustments.

### **For Black and White Negatives (Double-X)**

1. **Load the negative scan**.
2. **Add the Film Neg Invert plugin**.
3. **Configure Input Type** as needed.
4. **Select Film Type**: "Double-X (B&W)".
5. **Detect the mask** using **Analyze Mask**.
6. **Adjust Film Gamma** (try 0.5 for Double-X).
7. **Enable Color Invert**.
8. **Adjust Exposure** as needed.

## ---

**Usage Examples**

### **Example 1: Vision3 500T RAW Scan**

```
Input Type: Linear (RAW)
Film Type: Vision3 (Color)
Mask Color: [Use Analyze Mask]
Film Gamma: 0.6
Exposure: 1.2
Color Invert: [X] Enabled
```

### **Example 2: Double-X DPX Log**

```
Input Type: Log (Cineon/DPX)
Film Type: Double-X (B&W)
Mask Color: R: 0.75, G: 0.45, B: 0.25
Film Gamma: 0.5
Exposure: 1.0
Color Invert: [X] Enabled
```

### **Example 3: Color Correction with RGB Factors**

If the resulting image has a blue dominance:

```
Red Factor: 1.2
Green Factor: 1.0
Blue Factor: 0.9
```

## ---

**Troubleshooting**

### **Problem: Plugin doesn't appear in DaVinci Resolve**

**Possible causes:**
* Incorrect installation.
* Insufficient permissions.
* Resolve not restarted.

**Solutions:**
1. Verify the file is at:
   C:\\Program Files\\Common Files\\OFX\\Plugins\\
   FilmNegInvert.ofx.bundle\\Contents\\Win64\\FilmNegInvert.ofx
2. Run the installer as Administrator.
3. Restart DaVinci Resolve.
4. Check the Resolve console for error messages.

### **Problem: Image becomes completely white or black**

**Possible causes:**
* Incorrect Mask Color.
* Exposure too high or too low.
* Color Invert not enabled.

**Solutions:**
1. Click **Analyze Mask** to detect the correct color.
2. Reduce the Exposure value (try 1.0).
3. Verify that **Color Invert** is enabled.

### **Problem: Distorted or unrealistic colors**

**Possible causes:**
* Incorrect Mask Color.
* Incorrect Input Type.
* Inadequate Film Gamma.

**Solutions:**
1. Verify that Input Type matches the scan format.
2. Readjust Mask Color using the picker.
3. Try different Film Gamma values (0.5 - 0.7).

### **Problem: Slow performance**

**Possible causes:**
* Very high resolution.
* Overloaded system.

**Solutions:**
1. Use proxies for editing.
2. Reduce timeline resolution during adjustment.
3. Apply the plugin only to final clips.

## ---

**Tips and Best Practices**

### **1. Choosing the Analysis Area**

When using **Analyze Mask**, select an area that represents the film base:
* **Ideal area**: Between film perforations.
* **Avoid**: Image areas or dark edges.
* **Tip**: Typical mask color is reddish-orange.

### **2. Recommended Adjustment Order**

1. Set Input Type and Film Type.
2. Configure Mask Color.
3. Adjust Film Gamma.
4. Enable Color Invert.
5. Adjust Exposure.
6. Fine-tuning with RGB factors.

### **3. Reference Values**

| Film Type     | Input Type | Film Gamma | Typical Mask Color |
|---------------|------------|------------|-------------------|
| Vision3 50D   | Linear    | 0.6        | R: 0.75-0.85      |
| Vision3 250D  | Linear    | 0.6        | R: 0.75-0.85      |
| Vision3 200T  | Linear    | 0.6        | R: 0.75-0.85      |
| Vision3 500T  | Linear    | 0.6        | R: 0.75-0.85      |
| Double-X      | Linear    | 0.5        | R: 0.70-0.80      |

### **4. Saving Presets**

To save frequently used configurations:
1. Adjust the desired parameters.
2. Right-click on the node.
3. Select **Save Node Preset**.
4. Give it a descriptive name (e.g., "Vision3 500T RAW").

### **5. Integration with Resolve Workflow**

Film Neg Invert works best as the **first node** in the color chain:

```
[Source] → [Film Neg Invert] → [Color Correction] → [Output]
```

This allows you to perform traditional color correction on the already inverted image.

## ---

**Support**

To report issues or suggest improvements:

* **Technical documentation**: See `docs/PROJECT_OVERVIEW.md`.
* **Changelog**: See `CHANGELOG.md`.

---

**VAI CORINTHIANS!!** ⚫⚪
