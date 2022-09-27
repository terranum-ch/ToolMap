#include "framemain.h"


extern const char *feature_toc_MAJOR_VERSION;
extern const char *feature_toc_MINOR_VERSION;
extern const char *GIT_REV;
extern const char *GIT_TAG;
extern const char *GIT_BRANCH;
extern const char *GIT_NUMBER;

FrameMain::FrameMain(const wxString &title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(900, 700)) {
    wxInitAllImageHandlers();
    _create_controls();
}

void FrameMain::_create_controls() {

}

