#include "import_3d_model.h"

#include <editor/project.h>
#include <QFileDialog>

using namespace element::ui;

import_3d_model::import_3d_model(QWidget* parent) : QDialog(parent) {
    setupUi(this);
    setFixedSize(minimumSize());
    setWindowFlags(Qt::Sheet | Qt::Dialog);
    connect(source_browse, SIGNAL(clicked()), this, SLOT(browse_model()), Qt::ConnectionType::DirectConnection);
    connect(dest_browse, SIGNAL(clicked()), this, SLOT(browse_destination()), Qt::ConnectionType::DirectConnection);
    connect(this, SIGNAL(accepted()), this, SLOT(import()), Qt::ConnectionType::DirectConnection);
}

void import_3d_model::browse_model() {
    QStringList filters = {
        QCoreApplication::translate("element-editor", "3D models") + " (*.amf *.3ds *.ac *.ase *.assbin *.b3d *.bvh *.collada *.dxf *.csm *.hmp *.irrmesh *.iqm *.irr *.lwo *.lws *.m3d *.md2 *.md3 *.md5 *.mdc *.mdl *.nff *.ndo *.off *.obj *.ogre *.opengex *.ply *.ms3d *.cob *.blend *.ifc *.xgl *.fbx *.q3d *.q3bsp *.raw *.sib *.smd *.stl *.terragen *.3d *.x *.x3d *.gltf *.3mf *.mmd)",
        "AMF (*.amf)",
        "3D Studio Max (*.3ds)",
        "AC3D (*.ac)",
        "ASCI Scene Export (*.ase)",
        "ASSIMP Binary (*.assbin)",
        "BlitzBasic (*.b3d)",
        "Biovision Hierarchy (*.bvh)",
        "COLLADA (*.dae)",
        "AutoCAD DXF (*.dxf)",
        "CharacterStudio Motion (*.csm)",
        "HMP (*.hmp)",
        "Irrlicht Mesh (*.irrmesh)",
        "Inter-Quake Model (*.iqm)",
        "Irrlicht Scene (*.irr)",
        "LightWave (*.lwo)",
        "LightWave Scene (*.lws)",
        "Milkshape 3D (*.m3d)",
        "Quake II Model (*.md2)",
        "Quake III Model (*.md3)",
        "Doom 3 Model (*.md5)",
        "3D GameStudio Model (*.mdc)",
        "3D GameStudio Model (*.mdl)",
        "Neutral File Format (*.nff)",
        "Nendo (*.ndo)",
        "Object File Format (*.off)",
        "Wavefront OBJ (*.obj)",
        "Ogre (*.ogre)",
        "Open Game Engine Exchange (*.opengex)",
        "Polygon File Format (*.ply)",
        "Milkshape 3D (*.ms3d)",
        "TrueSpace (*.cob)",
        "Blender (*.blend)",
        "IFC (*.ifc)",
        "XGL (*.xgl)",
        "Autodesk FBX (*.fbx)",
        "Quick3D (*.q3d)",
        "Quake III BSP (*.q3bsp)",
        "Raw Triangles (*.raw)",
        "SIB (*.sib)",
        "Valve Source Model (*.smd)",
        "Stereolithography (*.stl)",
        "Terragen Terrain (*.terragen)",
        "Unreal (*.3d)",
        "DirectX (*.x)",
        "Extensible 3D (*.x3d)",
        "glTF (*.gltf)",
        "3D Manufacturing Format (*.3mf)",
        "MikuMikuDance (*.mmd)"
    };
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilters(filters);
    dialog.setDirectory(project::project_assets_path.generic_string().c_str());
    dialog.setWindowTitle(QCoreApplication::translate("element-editor", "Select source 3D model"));
    if (dialog.exec()) {
        auto files = dialog.selectedFiles();
        if (!files.isEmpty()) {
            source_edit->setText(files.first());
        }
    }
}

void import_3d_model::browse_destination() {
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setDirectory(project::project_assets_path.generic_string().c_str());
    dialog.setWindowTitle(QCoreApplication::translate("element-editor", "Select destination directory"));
    if (dialog.exec()) {
        auto files = dialog.selectedFiles();
        if (!files.isEmpty()) {
            dest_edit->setText(files.first());
        }
    }
}

void import_3d_model::import() {
    //TODO: Implement 3D model import
}