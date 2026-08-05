# TODO — ILC

## Refatoração do src/

- [ ] Separar `src/ilc.c` em módulos: `main.c`, `cmd_build.c`, `cmd_run.c`, `cmd_new.c`
- [ ] Integrar `ilcToml.h` no `cmd_build` (substituir parsing manual de TOML)
- [ ] Atualizar `ilc.toml` para usar seções `[project]`

## Funcionalidades planejadas

- [ ] `ilc run <args>` — suporte a argumentos na execução
- [ ] `ilc build` — busca recursiva de `.c` em `src/`
- [ ] Implementar `ilc add <lib>` — download de libs do ecossistema
- [ ] Implementar `ilc remove <lib>`

## Makefile

- [ ] Atualizar Makefile para listar múltiplos `.c` explicitamente
- [ ] Adicionar target `test`

## Testes

- [ ] Criar `tests/run_tests.sh`

## Lib de erros (`ilcError.h`)

- [ ] Criar `include/ilcError.h` — framework de erros com mensagens e localização

### Ideia de design

Lib header-only seguindo o padrão X-Macro já usado em `ilcArray.h`/`ilcFile.h`. O usuário define uma única lista de erros e a lib gera o enum + tabela de mensagens automaticamente.

**Uso pelo usuário:**
```c
#define ILCERR_LIST(X) \
    X(OK, "sem erro") \
    X(NEW_DIR_EXISTS, "diretório já existe") \
    X(NEW_MKDIR, "falha ao criar diretório") \
    X(BUILD_TOML_OPEN, "não foi possível abrir ilc.toml") \
    X(BUILD_GCC, "falha ao executar gcc") \
    X(RUN_BIN_MISSING, "build/run não encontrado")

#define ILCERR_IMPLEMENTATION
#include "ilcError.h"
```

**A lib gera:**
- `typedef enum { ILCERR_OK, ILCERR_NEW_DIR_EXISTS, ..., ILCERR_COUNT } ilcErr_t;`
- `const char *ilcErrStr(ilcErr_t code);` — lookup na tabela de mensagens
- `int ilcErrReport(ilcErr_t code, const char *file, int line);` — imprime `file:line → msg` e retorna o código

**Macros de report:**
```c
#define ILC_RET(err)  return ilcErrReport(err, __FILE__, __LINE__)
#define ILC_ERR(err)  ilcErrReport(err, __FILE__, __LINE__)
#define ILC_PERR(ctx) ilcErrReportErrno(ctx, __LINE__)  // para erros de SO (perror)
```

**Pontos a decidir:**
- Onde mora: `include/` aqui ou no repositório `ilcLibs` (é genérica, faria sentido lá)
- Prefixo: `ILCERR_`/`ilcErr` (genérico) vs `ILC_ERR_`/`ilc` (específico do CLI)
- Se `ILC_PERR` deve integrar `perror`/`errno` ou manter `perror` manual
