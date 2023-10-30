#include <asset/importer.h>
#include <asset/importers/helpers/shader.h>
#include <utils/uuid.h>

static void frag_importer(const element::uuid& id) {
    element::importers::helpers::shader_compile(id, element::shader::shader_stage::fragment);
}

ELM_REGISTER_IMPORTER("frag", frag_importer)