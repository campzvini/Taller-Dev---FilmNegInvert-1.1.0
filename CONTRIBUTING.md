# Contributing

Taller Lab: https://tallerlab.com/

## PT-BR

English version bellow.

### Convite

Se voce quiser contribuir com este projeto, contribua.

A ideia aqui e simples: se algo puder ficar melhor e sua contribuicao ajudar, ela e bem-vinda. Correcao de bug, limpeza de codigo, melhoria de performance, documentacao, testes, sugestoes de fluxo, suporte a outros formatos ou experimentos com GPU, tudo isso faz sentido.

### Contexto

`Film Neg Invert` e um projeto pequeno, pratico e em evolucao. Ele esta sendo testado em uso real e ainda ha espaco para refinamento tecnico, especialmente em performance, experiencia de uso e possivel caminho futuro de GPU.

### Como colaborar

Voce pode colaborar da forma que fizer mais sentido para voce:

- abrindo uma issue;
- propondo um patch;
- enviando um pull request;
- apontando inconsistencias na documentacao;
- testando em materiais reais e relatando comportamento.

Nao existe exigencia de seguir um ritual especifico alem de clareza minima para que outra pessoa consiga entender o que foi feito e por que.

### Build

Build principal:

```powershell
cmake -S . -B out/build/x64-Release -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build out/build/x64-Release --config RelWithDebInfo
```

Se o ambiente do Visual Studio nao estiver carregado:

```powershell
cmd /c "call \"C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat\" -arch=x64 && cmake --build out\build\x64-Release --config RelWithDebInfo"
```

### O que ajuda bastante

- explicar o problema de forma objetiva;
- descrever o efeito esperado da mudanca;
- manter a documentacao coerente quando a interface ou o comportamento mudar;
- avisar se a mudanca foi validada no DaVinci Resolve;
- atualizar `dist/FilmNegInvert.ofx` apenas quando houver um build realmente pronto para distribuicao.

### Nota

Algumas escolhas de linguagem, headers e assinatura refletem a forma como o projeto e publicado pelo autor. Isso nao pretende ditar como outras pessoas devem trabalhar. O que importa aqui e a contribuicao ser util, clara e funcionar bem.

---

## EN

### Invitation

If you want to contribute to this project, contribute.

The idea here is simple: if something can be improved and your contribution helps, it is welcome. Bug fixes, code cleanup, performance work, documentation, tests, workflow suggestions, support for other formats, or GPU experiments all make sense here.

### Context

`Film Neg Invert` is a small, practical project that is still evolving. It is being tested in real use, and there is still room for technical refinement, especially around performance, usability, and a possible future GPU path.

### How to contribute

You can contribute in whatever way makes the most sense for you:

- open an issue;
- propose a patch;
- send a pull request;
- point out documentation inconsistencies;
- test with real material and report behavior.

There is no strict ritual here beyond basic clarity so another person can understand what changed and why.

### Build

Main build:

```powershell
cmake -S . -B out/build/x64-Release -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build out/build/x64-Release --config RelWithDebInfo
```

If the Visual Studio environment is not loaded:

```powershell
cmd /c "call \"C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat\" -arch=x64 && cmake --build out\build\x64-Release --config RelWithDebInfo"
```

### What helps a lot

- explain the problem clearly;
- describe the intended effect of the change;
- keep the documentation aligned when the UI or behavior changes;
- mention whether the change was validated in DaVinci Resolve;
- update `dist/FilmNegInvert.ofx` only when there is a build that is truly ready for distribution.

### Note

Some language choices, header formatting, and signature lines reflect how the project is published by its author. That is not meant to dictate how other people must work. What matters here is that the contribution is useful, clear, and works well.
