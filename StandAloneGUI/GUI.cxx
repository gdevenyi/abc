// generated by Fast Light User Interface Designer (fluid) version 1.0109

#include "GUI.h"
#include "Fl/Fl_File_Chooser.h"
#include "itkExceptionObject.h"
#include "runEMS.h"
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>

void GUI::cb_Close_i(Fl_Button*, void*) {
  aboutwindow->hide();
}
void GUI::cb_Close(Fl_Button* o, void* v) {
  ((GUI*)(o->parent()->user_data()))->cb_Close_i(o,v);
}

void GUI::cb_OK_i(Fl_Return_Button*, void*) {
  int pos = imageBrowser->value();

imageOrientBrowser->text(pos, orientInput->value());

orientwindow->hide();
orientwindow->set_non_modal();
}
void GUI::cb_OK(Fl_Return_Button* o, void* v) {
  ((GUI*)(o->parent()->user_data()))->cb_OK_i(o,v);
}

void GUI::cb_Next_i(Fl_Button*, void*) {
  tab1->hide();
tab3->hide();
tab4->hide();

tab2->show();
}
void GUI::cb_Next(Fl_Button* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_Next_i(o,v);
}

void GUI::cb_Change_i(Fl_Button*, void*) {
  char* dir = fl_dir_chooser("Select atlas directory", NULL, 0);
if (dir == NULL)
  return;
atlasdirOutput->value(dir);
}
void GUI::cb_Change(Fl_Button* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_Change_i(o,v);
}

void GUI::cb_Add_i(Fl_Button*, void*) {
  char* fn = fl_file_chooser("Add image...", "{*.hdr,*.gipl,*.mha,*.mhd}", NULL, 0);
if (fn == NULL)
  return;

const char* deforient = "RAI";

imageBrowser->add(fn);
imageOrientBrowser->add(deforient);
}
void GUI::cb_Add(Fl_Button* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_Add_i(o,v);
}

void GUI::cb_Remove_i(Fl_Button*, void*) {
  if (imageBrowser->size() == 0)
  return;
if (imageBrowser->value() == 0)
  return;

int pos = imageBrowser->value();

imageBrowser->remove(pos);
imageOrientBrowser->remove(pos);
}
void GUI::cb_Remove(Fl_Button* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_Remove_i(o,v);
}

void GUI::cb_Next1_i(Fl_Button*, void*) {
  tab1->hide();
tab2->hide();
tab4->hide();

tab3->show();
}
void GUI::cb_Next1(Fl_Button* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_Next1_i(o,v);
}

void GUI::cb_Change1_i(Fl_Button*, void*) {
  if (imageBrowser->size() == 0)
  return;
if (imageBrowser->value() == 0)
  return;

int pos = imageBrowser->value();

const char* curr = imageOrientBrowser->text(pos);
orientInput->value(curr);

orientwindow->show();
orientwindow->set_modal();
}
void GUI::cb_Change1(Fl_Button* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_Change1_i(o,v);
}

Fl_Menu_Item GUI::menu_filterMethodChoice[] = {
 {"Curvature flow", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Grad aniso diffusion", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0}
};

Fl_Menu_Item GUI::menu_formatChoice[] = {
 {"Analyze", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"GIPL", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Meta", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Nrrd", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0}
};

void GUI::cb_Change2_i(Fl_Button*, void*) {
  char* dir = fl_dir_chooser("Select output directory", NULL, 0);
if (dir == NULL)
  return;
outdirOutput->value(dir);
}
void GUI::cb_Change2(Fl_Button* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_Change2_i(o,v);
}

void GUI::cb_Next2_i(Fl_Button*, void*) {
  tab1->hide();
tab2->hide();
tab3->hide();

tab4->show();
}
void GUI::cb_Next2(Fl_Button* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_Next2_i(o,v);
}

void GUI::cb_Run_i(Fl_Button*, void*) {
  mainwindow->hide();
runwindow->show();

bool debugflag = (debugCheck->value() == 1);
bool writeflag = !(nowriteCheck->value() == 1);

  EMSParameters::Pointer p = this->GetParameters();

try
{
  runEMS(p, debugflag, writeflag);
}
catch (itk::ExceptionObject& err)
{
  std::string s = "ITK exception caught:\n";
  s += std::string(err.what());
  fl_alert(s.c_str());
}
catch (std::exception& e)
{
  std::string s = "Exception caught:\n";
  s += std::string(e.what());
  fl_alert(s.c_str());
}
catch (std::string& s)
{
  fl_alert(s.c_str());
}
catch (...)
{
  fl_alert("Unknown exception occured");
}

runwindow->hide();
mainwindow->show();
}
void GUI::cb_Run(Fl_Button* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_Run_i(o,v);
}

void GUI::cb_New_i(Fl_Menu_*, void*) {
  // Reset values and GUI elements
this->Initialize();
}
void GUI::cb_New(Fl_Menu_* o, void* v) {
  ((GUI*)(o->parent()->user_data()))->cb_New_i(o,v);
}

void GUI::cb_Load_i(Fl_Menu_*, void*) {
  char* fn = fl_file_chooser("Load parameter file...", "*.xml", NULL, 0);
if (fn == NULL)
  return;
EMSParameters::Pointer p = readEMSParametersXML(fn);
if (p.IsNull())
{
  fl_alert("Load failed");
  return;
}
this->SetGUIElements(p);

// Back to first tab
tab4->hide();
tab2->hide();
tab3->hide();
tab1->show();
}
void GUI::cb_Load(Fl_Menu_* o, void* v) {
  ((GUI*)(o->parent()->user_data()))->cb_Load_i(o,v);
}

void GUI::cb_Save_i(Fl_Menu_*, void*) {
  char* fn = fl_file_chooser("Save parameter file...", "*.xml", NULL, 0);
if (fn == NULL)
  return;

EMSParameters::Pointer p = this->GetParameters();

if (!writeEMSParametersXML(fn, p))
  fl_alert("Save failed: bad parameters?");
}
void GUI::cb_Save(Fl_Menu_* o, void* v) {
  ((GUI*)(o->parent()->user_data()))->cb_Save_i(o,v);
}

void GUI::cb_Quit_i(Fl_Menu_*, void*) {
  exit(0);
}
void GUI::cb_Quit(Fl_Menu_* o, void* v) {
  ((GUI*)(o->parent()->user_data()))->cb_Quit_i(o,v);
}

void GUI::cb_About_i(Fl_Menu_*, void*) {
  aboutwindow->show();
}
void GUI::cb_About(Fl_Menu_* o, void* v) {
  ((GUI*)(o->parent()->user_data()))->cb_About_i(o,v);
}

Fl_Menu_Item GUI::menu_[] = {
 {"File", 0,  0, 0, 64, FL_NORMAL_LABEL, 0, 14, 0},
 {"New", 0,  (Fl_Callback*)GUI::cb_New, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Load", 0,  (Fl_Callback*)GUI::cb_Load, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Save as...", 0,  (Fl_Callback*)GUI::cb_Save, 0, 128, FL_NORMAL_LABEL, 0, 14, 0},
 {"Quit", 0,  (Fl_Callback*)GUI::cb_Quit, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {"Help", 0,  0, 0, 64, FL_NORMAL_LABEL, 0, 14, 0},
 {"About", 0,  (Fl_Callback*)GUI::cb_About, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {0,0,0,0,0,0,0,0,0}
};

Fl_Double_Window* GUI::MakeWindow() {
  { aboutwindow = new Fl_Double_Window(612, 668, "ABC: About");
    aboutwindow->user_data((void*)(this));
    { Fl_Box* o = new Fl_Box(5, 5, 600, 600, "\r\nABC (Atlas Based Classification) GUI v1.0\r\nNormal brain segmentation fr\
om MRI\r\n\r\nMarcel Prastawa\r\nprastawa@@sci.utah.edu\r\nhttp://www.sci.utah\
.edu/~prastawa\r\n\r\nThis software is provided for research purposes only\r\n\
\r\nImplemented using FLTK and ITK\r\nhttp://www.fltk.org\r\nhttp://www.itk.or\
g\r\n\r\nBased on the following papers:\r\n\r\nMaes, F., Collignon, A., Vander\
meulen, D., Marchal, G., Suetens, P. \r\nMultimodality image registration by m\
aximization of mutual information.\r\nIEEE TMI 1997; 16(2):187-198\r\n\r\nVan \
Leemput K, Maes F, Vandermeulen D, Suetens P. Automated model based\r\ntissue \
classification of MR images of the brain. IEEE TMI 1999; 18:897-908\r\n\r\nVan\
 Leemput K, Maes F, Vandermeulen D, Suetens P. Automated model based\r\nbias f\
ield correction of MR images of the brain. IEEE TMI 1999; 18:885-896\r\n");
      o->box(FL_FLAT_BOX);
      o->color((Fl_Color)26);
      o->align(FL_ALIGN_TOP_LEFT|FL_ALIGN_INSIDE);
    } // Fl_Box* o
    { Fl_Button* o = new Fl_Button(270, 620, 80, 30, "Close");
      o->callback((Fl_Callback*)cb_Close);
    } // Fl_Button* o
    aboutwindow->end();
  } // Fl_Double_Window* aboutwindow
  { orientwindow = new Fl_Double_Window(285, 110, "Image Orientation");
    orientwindow->user_data((void*)(this));
    { orientInput = new Fl_Input(95, 20, 180, 30, "Orientation:");
    } // Fl_Input* orientInput
    { Fl_Return_Button* o = new Fl_Return_Button(175, 65, 100, 30, "OK");
      o->callback((Fl_Callback*)cb_OK);
    } // Fl_Return_Button* o
    orientwindow->end();
  } // Fl_Double_Window* orientwindow
  { runwindow = new Fl_Double_Window(361, 246, "ABC: Running...");
    runwindow->user_data((void*)(this));
    { Fl_Box* o = new Fl_Box(25, 25, 300, 200, "Segmentation in progress, please wait...");
      o->box(FL_FLAT_BOX);
      o->color((Fl_Color)247);
    } // Fl_Box* o
    runwindow->end();
  } // Fl_Double_Window* runwindow
  { mainwindow = new Fl_Double_Window(600, 535, "ABC");
    mainwindow->user_data((void*)(this));
    { Fl_Tabs* o = new Fl_Tabs(0, 35, 620, 500);
      o->box(FL_UP_BOX);
      { tab1 = new Fl_Group(10, 65, 595, 450, "Step 1");
        tab1->align(129);
        { Fl_Button* o = new Fl_Button(500, 480, 80, 30, "Next");
          o->callback((Fl_Callback*)cb_Next);
        } // Fl_Button* o
        { suffixInput = new Fl_Input(64, 110, 300, 30, "Suffix");
        } // Fl_Input* suffixInput
        { atlasdirOutput = new Fl_Output(127, 203, 365, 30, "Atlas Directory:");
          atlasdirOutput->color(FL_DARK1);
        } // Fl_Output* atlasdirOutput
        { Fl_Box* o = new Fl_Box(25, 74, 580, 30, "String used to tag all output files (e.g. case1)");
          o->box(FL_FLAT_BOX);
          o->color((Fl_Color)175);
          o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
        } // Fl_Box* o
        { Fl_Box* o = new Fl_Box(25, 164, 580, 30, "Where we can find the spatial priors and template");
          o->box(FL_FLAT_BOX);
          o->color((Fl_Color)175);
          o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
        } // Fl_Box* o
        { Fl_Box* o = new Fl_Box(25, 319, 580, 30, "Global class priors");
          o->box(FL_FLAT_BOX);
          o->color((Fl_Color)175);
          o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
        } // Fl_Box* o
        { prior1Input = new Fl_Input(55, 354, 40, 30, "WM");
        } // Fl_Input* prior1Input
        { prior2Input = new Fl_Input(130, 354, 40, 30, "GM");
        } // Fl_Input* prior2Input
        { prior3Input = new Fl_Input(210, 354, 40, 30, "CSF");
        } // Fl_Input* prior3Input
        { prior4Input = new Fl_Input(300, 354, 40, 30, "Other");
        } // Fl_Input* prior4Input
        { Fl_Button* o = new Fl_Button(500, 203, 80, 30, "Change");
          o->callback((Fl_Callback*)cb_Change);
        } // Fl_Button* o
        { Fl_Box* o = new Fl_Box(25, 243, 580, 30, "Orientation of atlas images, permutations of R/L, A/P, I/S (e.g. RAI, ASR)");
          o->box(FL_FLAT_BOX);
          o->color((Fl_Color)175);
          o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
        } // Fl_Box* o
        { atlasOrientInput = new Fl_Input(135, 278, 300, 30, "Atlas Orientation");
        } // Fl_Input* atlasOrientInput
        { warpAtlasButton = new Fl_Check_Button(25, 431, 25, 25, "Warp atlas to subject");
          warpAtlasButton->down_box(FL_DOWN_BOX);
        } // Fl_Check_Button* warpAtlasButton
        { Fl_Box* o = new Fl_Box(25, 393, 580, 30, "Deformable registration of atlas to subject");
          o->box(FL_FLAT_BOX);
          o->color((Fl_Color)175);
          o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
        } // Fl_Box* o
        { warpInput_nx = new Fl_Input(261, 429, 40, 30, "Grid size:");
        } // Fl_Input* warpInput_nx
        { warpInput_ny = new Fl_Input(320, 429, 40, 30, "X");
        } // Fl_Input* warpInput_ny
        { warpInput_nz = new Fl_Input(376, 430, 40, 30, "X");
        } // Fl_Input* warpInput_nz
        tab1->end();
      } // Fl_Group* tab1
      { tab2 = new Fl_Group(10, 65, 595, 450, "Step 2");
        tab2->hide();
        { imageBrowser = new Fl_Browser(25, 128, 450, 120, "Images:");
          imageBrowser->type(2);
          imageBrowser->align(FL_ALIGN_TOP_LEFT);
        } // Fl_Browser* imageBrowser
        { Fl_Button* o = new Fl_Button(25, 254, 100, 30, "Add");
          o->callback((Fl_Callback*)cb_Add);
        } // Fl_Button* o
        { Fl_Button* o = new Fl_Button(140, 254, 100, 30, "Remove");
          o->callback((Fl_Callback*)cb_Remove);
        } // Fl_Button* o
        { Fl_Box* o = new Fl_Box(25, 299, 580, 30, "Set number of iterations to zero to disable filtering, keep time step value s\
mall for stability");
          o->box(FL_FLAT_BOX);
          o->color((Fl_Color)175);
          o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
        } // Fl_Box* o
        { Fl_Box* o = new Fl_Box(25, 75, 580, 30, "Specify images to be used for segmentation, all are registered to the first i\
mage in the list");
          o->box(FL_FLAT_BOX);
          o->color((Fl_Color)175);
          o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
        } // Fl_Box* o
        { Fl_Box* o = new Fl_Box(25, 329, 200, 30, "Filtering parameters");
          o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
        } // Fl_Box* o
        { filterIterInput = new Fl_Input(100, 359, 80, 30, "Iterations:");
        } // Fl_Input* filterIterInput
        { filterDtInput = new Fl_Input(259, 359, 80, 30, "Time Step:");
        } // Fl_Input* filterDtInput
        { Fl_Button* o = new Fl_Button(500, 480, 80, 30, "Next");
          o->callback((Fl_Callback*)cb_Next1);
        } // Fl_Button* o
        { Fl_Box* o = new Fl_Box(25, 399, 580, 30, "Maximum degree of the bias field polynomial");
          o->box(FL_FLAT_BOX);
          o->color((Fl_Color)175);
          o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
        } // Fl_Box* o
        { biasDegreeInput = new Fl_Input(180, 435, 80, 30, "Maximum bias degree:");
        } // Fl_Input* biasDegreeInput
        { imageOrientBrowser = new Fl_Browser(485, 130, 80, 120, "Orientations:");
          imageOrientBrowser->align(FL_ALIGN_TOP_LEFT);
        } // Fl_Browser* imageOrientBrowser
        { Fl_Button* o = new Fl_Button(340, 255, 135, 30, "Change Orientation");
          o->callback((Fl_Callback*)cb_Change1);
        } // Fl_Button* o
        { filterMethodChoice = new Fl_Choice(400, 358, 180, 30, "Method:");
          filterMethodChoice->down_box(FL_BORDER_BOX);
          filterMethodChoice->menu(menu_filterMethodChoice);
        } // Fl_Choice* filterMethodChoice
        tab2->end();
      } // Fl_Group* tab2
      { tab3 = new Fl_Group(10, 65, 600, 450, "Step 3");
        tab3->hide();
        { outdirOutput = new Fl_Output(145, 116, 350, 30, "Output Directory:");
          outdirOutput->color(FL_DARK1);
        } // Fl_Output* outdirOutput
        { formatChoice = new Fl_Choice(175, 196, 100, 30, "Output Image Format:");
          formatChoice->down_box(FL_BORDER_BOX);
          formatChoice->menu(menu_formatChoice);
        } // Fl_Choice* formatChoice
        { Fl_Box* o = new Fl_Box(30, 77, 580, 30, "Where to store all output files");
          o->box(FL_FLAT_BOX);
          o->color((Fl_Color)175);
          o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
        } // Fl_Box* o
        { Fl_Box* o = new Fl_Box(30, 157, 580, 30, "Format of output images");
          o->box(FL_FLAT_BOX);
          o->color((Fl_Color)175);
          o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
        } // Fl_Box* o
        { Fl_Button* o = new Fl_Button(500, 116, 80, 30, "Change");
          o->callback((Fl_Callback*)cb_Change2);
        } // Fl_Button* o
        { Fl_Button* o = new Fl_Button(500, 480, 80, 30, "Next");
          o->callback((Fl_Callback*)cb_Next2);
        } // Fl_Button* o
        tab3->end();
      } // Fl_Group* tab3
      { tab4 = new Fl_Group(10, 65, 595, 450, "Step 4");
        tab4->hide();
        { Fl_Button* o = new Fl_Button(500, 480, 80, 30, "Run");
          o->callback((Fl_Callback*)cb_Run);
        } // Fl_Button* o
        { debugCheck = new Fl_Check_Button(25, 114, 500, 30, "Write debug messages to terminal");
          debugCheck->down_box(FL_DOWN_BOX);
        } // Fl_Check_Button* debugCheck
        { nowriteCheck = new Fl_Check_Button(25, 148, 500, 30, "Minimal image write (only label image gets written)");
          nowriteCheck->down_box(FL_DOWN_BOX);
        } // Fl_Check_Button* nowriteCheck
        { Fl_Box* o = new Fl_Box(25, 77, 580, 30, "Additional run options, leave them unchecked unless necessary");
          o->box(FL_FLAT_BOX);
          o->color((Fl_Color)175);
          o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
        } // Fl_Box* o
        tab4->end();
      } // Fl_Group* tab4
      o->end();
    } // Fl_Tabs* o
    { Fl_Menu_Bar* o = new Fl_Menu_Bar(0, 0, 620, 30);
      o->menu(menu_);
    } // Fl_Menu_Bar* o
    mainwindow->end();
  } // Fl_Double_Window* mainwindow
  return mainwindow;
}

void GUI::Initialize() {
  EMSParameters::Pointer p = EMSParameters::New();
this->SetGUIElements(p);

tab2->hide();
tab3->hide();
tab4->hide();

tab1->show();
}

EMSParameters::Pointer GUI::GetParameters() {
  EMSParameters::Pointer p = EMSParameters::New();

p->SetSuffix(suffixInput->value());

p->SetAtlasDirectory(atlasdirOutput->value());
p->SetAtlasOrientation(atlasOrientInput->value());

p->SetOutputDirectory(outdirOutput->value());

p->SetPrior1(atof(prior1Input->value()));
p->SetPrior2(atof(prior2Input->value()));
p->SetPrior3(atof(prior3Input->value()));
p->SetPrior4(atof(prior4Input->value()));

int f = formatChoice->value();
if (f == 0)
  p->SetOutputFormat("Analyze");
else if (f == 1)
  p->SetOutputFormat("GIPL");
else if (f == 3)
  p->SetOutputFormat("Nrrd");
else
  p->SetOutputFormat("Meta");

for (int i = 0; i < imageBrowser->size(); i++)
  p->AddImage(imageBrowser->text(i+1), imageOrientBrowser->text(i+1));

p->SetFilterIterations(atoi(filterIterInput->value()));
p->SetFilterTimeStep(atof(filterDtInput->value()));

int m = filterMethodChoice->value();
if (m == 0)
  p->SetFilterMethod("Curvature flow");
else
  p->SetFilterMethod("Grad aniso diffusion");

p->SetMaxBiasDegree(atoi(biasDegreeInput->value()));

p->SetDoAtlasWarp(warpAtlasButton->value() != 0);

p->SetAtlasWarpGridX(atoi(warpInput_nx->value()));
p->SetAtlasWarpGridY(atoi(warpInput_ny->value()));
p->SetAtlasWarpGridZ(atoi(warpInput_nz->value()));

return p;
}

void GUI::SetGUIElements(EMSParameters::Pointer p) {
  suffixInput->value(p->GetSuffix().c_str());

atlasdirOutput->value(p->GetAtlasDirectory().c_str());
atlasOrientInput->value(p->GetAtlasOrientation().c_str());

outdirOutput->value(p->GetOutputDirectory().c_str());

formatChoice->value(2);
if (p->GetOutputFormat().compare("Analyze") == 0)
  formatChoice->value(0);
if (p->GetOutputFormat().compare("GIPL") == 0)
  formatChoice->value(1);
if (p->GetOutputFormat().compare("Nrrd") == 0)
  formatChoice->value(3);

imageBrowser->clear();
imageOrientBrowser->clear();
DynArray<std::string> images = p->GetImages();
DynArray<std::string> orients = p->GetImageOrientations();
for (unsigned int i = 0; i < images.GetSize(); i++)
{
  imageBrowser->add(images[i].c_str());
  imageOrientBrowser->add(orients[i].c_str());
}

{
std::stringstream oss;
oss << p->GetFilterIterations() << std::ends;
filterIterInput->value(oss.str().c_str());
}

{
std::stringstream oss;
oss << p->GetFilterTimeStep() << std::ends;
filterDtInput->value(oss.str().c_str());
}

std::string filtermethod = p->GetFilterMethod();
if (filtermethod.compare("Curvature flow") == 0)
  filterMethodChoice->value(0);
else
  filterMethodChoice->value(1);

{
std::stringstream oss;
oss << p->GetPrior1() << std::ends;
prior1Input->value(oss.str().c_str());
}

{
std::stringstream oss;
oss << p->GetPrior2() << std::ends;
prior2Input->value(oss.str().c_str());
}

{
std::stringstream oss;
oss << p->GetPrior3() << std::ends;
prior3Input->value(oss.str().c_str());
}

{
std::stringstream oss;
oss << p->GetPrior4() << std::ends;
prior4Input->value(oss.str().c_str());
}

{
std::stringstream oss;
oss << p->GetMaxBiasDegree() << std::ends;
biasDegreeInput->value(oss.str().c_str());
}

warpAtlasButton->value(p->GetDoAtlasWarp());

{
std::stringstream oss;
oss << p->GetAtlasWarpGridX() << std::ends;
warpInput_nx->value(oss.str().c_str());
}

{
std::stringstream oss;
oss << p->GetAtlasWarpGridY() << std::ends;
warpInput_ny->value(oss.str().c_str());
}

{
std::stringstream oss;
oss << p->GetAtlasWarpGridZ() << std::ends;
warpInput_nz->value(oss.str().c_str());
}
}