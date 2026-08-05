# Resumo do Projeto ILC (I Love C)

O **ILC (I Love C)** é um gerenciador de pacotes, scaffold de projetos e ecossistema de bibliotecas padrão para a linguagem C. Ele visa trazer a experiência de desenvolvimento ágil e conveniente de linguagens modernas (como Rust com `cargo` ou JavaScript com `npm`) para o ecossistema C, eliminando a complexidade típica de configuração de build e gerenciamento de dependências.

---

## 📂 Estrutura do Repositório

O repositório está organizado da seguinte forma:

- **[src/](file:///home/ilx159/Repositories/ilc/src)**: Código-fonte do CLI do ILC.
  - **[ilc.c](file:///home/ilx159/Repositories/ilc/src/ilc.c)**: Implementação principal do utilitário de linha de comando.
- **[include/](file:///home/ilx159/Repositories/ilc/include)**: O ecossistema de bibliotecas cabeçalho-único (*header-only*).
  - **[ilcArena.h](file:///home/ilx159/Repositories/ilc/include/ilcArena.h)**: Alocador de memória baseado em arena para controle eficiente e seguro de memória.
  - **[ilcArray.h](file:///home/ilx159/Repositories/ilc/include/ilcArray.h)**: Arrays dinâmicos genéricos utilizando X-Macros para suporte a múltiplos tipos.
  - **[ilcDir.h](file:///home/ilx159/Repositories/ilc/include/ilcDir.h)**: Funções de sistema para manipulação e listagem de diretórios (POSIX).
  - **[ilcFile.h](file:///home/ilx159/Repositories/ilc/include/ilcFile.h)**: Manipulação de arquivos, incluindo leitura e escrita tipada via X-Macros.
  - **[ilcMath.h](file:///home/ilx159/Repositories/ilc/include/ilcMath.h)**: Utilitários matemáticos rápidos (LUT de Seno/Cosseno, interpolação linear, funções de clamp/abs).
  - **[ilcString.h](file:///home/ilx159/Repositories/ilc/include/ilcString.h)**: Manipulação de strings dinâmicas (`str_t`).
  - **[ilcTerminalInput.h](file:///home/ilx159/Repositories/ilc/include/ilcTerminalInput.h)**: Leitura de teclado e controle de terminal no modo bruto (*raw mode*).
  - **[ilcTypes.h](file:///home/ilx159/Repositories/ilc/include/ilcTypes.h)**: Definições de apelidos de tipos comuns (`u8`, `i32`, etc.) e tipos para vetores matemáticos de 2D a 4D.
- **[tests/](file:///home/ilx159/Repositories/ilc/tests)**: Diretório destinado aos testes unitários do projeto (atualmente vazio).
- **[Makefile](file:///home/ilx159/Repositories/ilc/Makefile)**: Automação da compilação do binário do CLI `ilc`.
- **[build.sh](file:///home/ilx159/Repositories/ilc/build.sh)**: Script auxiliar de build rápida.
- **[ilc.toml](file:///home/ilx159/Repositories/ilc/ilc.toml)**: Configuração padrão para o CLI do próprio projeto.
- **[todo.md](file:///home/ilx159/Repositories/ilc/todo.md)**: Lista de tarefas pendentes.

---

## 🛠️ O CLI do ILC

O executável principal compilado a partir de `ilc.c` oferece comandos simplificados para gerenciar projetos em C:

1. **`ilc new <project_name>`**:
   Cria um novo diretório com a estrutura do projeto:
   ```text
   meu_projeto/
   ├── src/
   │   └── main.c
   ├── include/
   ├── build/
   └── ilc.toml
   ```
   Também pré-configura o `ilc.toml` e gera um arquivo `main.c` básico com o template "Hello World".

2. **`ilc build`**:
   Lê as configurações de flags do `ilc.toml` e todos os arquivos em `src/`, invocando o compilador `gcc` automaticamente para gerar o binário resultante em `build/run`.

3. **`ilc run`**:
   Executa o binário compilado localizado em `build/run`.

4. **`ilc add <lib_name>`** *(Planejado)*:
   Adicionará bibliotecas diretamente do ecossistema do ILC ao escopo do projeto.

---

## 📚 Detalhes do Ecossistema (Header-only Libraries)

Todas as bibliotecas fornecidas pelo ecossistema ILC são de arquivo único (*header-only*). Para utilizá-las, basta definir a macro correspondente antes de incluir o cabeçalho em um arquivo de código-fonte. Por exemplo:

```c
#define ILCFILE_IMPLEMENTATION
#include "ilcFile.h"
```

### Bibliotecas Implementadas:
- **[ilcTypes.h](file:///home/ilx159/Repositories/ilc/include/ilcTypes.h)**:
  Oferece tipos portáveis como `u8`, `u16`, `u32`, `u64`, `i8`, `i16`, `i32`, `i64`, `f32`, `f64`. Também inclui vetores geométricos estruturados de 2, 3 e 4 componentes em ponto flutuante e inteiros (como `vec2i`, `vec3f`, `vec4f`).
- **[ilcArena.h](file:///home/ilx159/Repositories/ilc/include/ilcArena.h)**:
  Implementa um alocador simples de região de memória para otimizar alocações rápidas e liberações em lote via `arena_t`.
- **[ilcArray.h](file:///home/ilx159/Repositories/ilc/include/ilcArray.h)**:
  Fornece arrays dinâmicos genéricos gerados através do padrão X-Macro. Isso permite ter vetores de tipos primitivos que crescem automaticamente de forma segura em C.
- **[ilcDir.h](file:///home/ilx159/Repositories/ilc/include/ilcDir.h)**:
  Encapsula chamadas de sistema POSIX para manipulação de pastas: `dirExists`, `dirOpen`, `dirDelete`, `dirList`, `createDir` e `getCurrentDir`.
- **[ilcFile.h](file:///home/ilx159/Repositories/ilc/include/ilcFile.h)**:
  Tratamento simplificado de leitura/escrita de arquivos, suportando leitura/escrita tipada (ex: `u8FileRead`, `i32FileWrite`) e ações comuns como `fileExists`, `fileDelete`, `fileRename`, `fileMove`, `fileCopy`.
- **[ilcMath.h](file:///home/ilx159/Repositories/ilc/include/ilcMath.h)**:
  Aceleração matemática com tabela de busca pré-calculada (Look-up Table - LUT) de 360 graus para `ilcSin` e `ilcCos`, além de funções estáticas embutidas para limites (`clampf`, `clampi`), valor absoluto (`absf`) e interpolação linear discreta (`interpolate`).
- **[ilcString.h](file:///home/ilx159/Repositories/ilc/include/ilcString.h)**:
  Estrutura dinâmica `str_t` que armazena tamanho e capacidade. Oferece operações comuns como concatenação (`strAppend`), remoção de caracteres (`strStrip`), divisão de texto (`strSplit`), remoção de espaços em branco nas bordas (`strTrim`), busca por substrings (`strContains`), verificação de prefixos (`strStartWith`) e localização de caracteres (`strFind`).
- **[ilcTerminalInput.h](file:///home/ilx159/Repositories/ilc/include/ilcTerminalInput.h)**:
  Permite ativar/desativar o modo bruto de terminal POSIX (`rawModeOn`/`rawModeOff`) para interceptar teclas individualmente e de forma síncrona/assíncrona sem necessidade do buffer de linha do Enter.

---

## 📈 Roadmap & Próximos Passos

De acordo com o `todo.md` e a documentação interna:
- [ ] Implementar suporte a argumentos passados dinamicamente no comando `ilc run`.
- [ ] Adaptar o comando `ilc build` para suportar a busca recursiva de arquivos dentro de subpastas do diretório `src/`.
- [ ] Desenvolver a biblioteca `error.h` de tratamento estruturado de erros.
- [ ] Desenvolver o repositório central de manifestos `.toml` de pacotes para o comando `ilc add` baixar dependências automaticamente.
