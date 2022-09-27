#include "app.h"

#include "framemain.h"

IMPLEMENT_APP(MainApp)

bool MainApp::OnInit() {
  if (!wxApp::OnInit()) {
    return false;
  }

  FrameMain *frame = new FrameMain("feature_toc");
  frame->Show(true);
  return true;
}
