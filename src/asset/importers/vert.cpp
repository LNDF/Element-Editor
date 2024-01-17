#include <asset/importer.h>
#include <asset/importers/helpers/shader.h>
#include <utils/uuid.h>

static void vert_importer(const element::uuid& id) {
    element::importers::helpers::shader_compile(id, element::render::shader_stage::vertex);
}

ELM_REGISTER_IMPORTER("vert", vert_importer, 1)