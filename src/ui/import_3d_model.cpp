#include "import_3d_model.h"

#include <QFileDialog>

using namespace element::ui;

import_3d_model::import_3d_model(QWidget* parent) : QDialog(parent) {
    setupUi(this);
    setFixedSize(minimumSize());
    setWindowFlags(Qt::Sheet | Qt::Dialog);
}

void import_3d_model::browse_model() {
    QStringList filters = {
        QCoreApplication::translate("element-editor", "3D Models") + " (*.amf *.3ds *.ac *.ase *.assbin *.b3d *.bvh *.collada *.dxf *.csm *.hmp *.irrmesh *.iqm *.irr *.lwo *.lws *.m3d *.md2 *.md3 *.md5 *.mdc *.mdl *.nff *.ndo *.off *.obj *.ogre *.opengex *.ply *.ms3d *.cob *.blend *.ifc *.xgl *.fbx *.q3d *.q3bsp *.raw *.sib *.smd *.stl *.terragen *.3d *.x *.x3d *.gltf *.3mf *.mmd)",
        tr("All Files (*)")
    };
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("3D Models (*.obj *.fbx *.dae)"));
    if (dialog.exec()) {
        auto files = dialog.selectedFiles();
        if (!files.isEmpty()) {
            source_edit->setText(files.first());
        }
    }
}