#include <Shiny/Engine.h>
#include <Shiny/GLIncludes.h>
#include <Shiny/Log.h>
#include <Shiny/Shiny.h>

#include <cstdlib>

using namespace Shiny;

class TestEngine : public Engine {
public:
   TestEngine() {}

   virtual ~TestEngine() {}

   virtual void tick(const float dt) override;

   virtual void render() override;
};

void TestEngine::tick(const float dt) {
   SPtr<const Controller> controller(getController(0));
   if (!controller) {
      return;
   }

   const ControllerValues &values = controller->values();

   // Axes
   if (values.leftThumbHorizontal != 0.0f) {
      LOG_DEBUG("Left thumb horizontal: " << values.leftThumbHorizontal);
   }
   if (values.leftThumbVertical != 0.0f) {
      LOG_DEBUG("Left thumb vertical: " << values.leftThumbVertical);
   }

   if (values.rightThumbHorizontal != 0.0f) {
      LOG_DEBUG("Right thumb horizontal: " << values.rightThumbHorizontal);
   }
   if (values.rightThumbVertical != 0.0f) {
      LOG_DEBUG("Right thumb vertical: " << values.rightThumbVertical);
   }

   if (values.leftTriggerAxis != -1.0f) {
      LOG_DEBUG("Left trigger axis: " << values.leftTriggerAxis);
   }
   if (values.rightTriggerAxis != -1.0f) {
      LOG_DEBUG("Right trigger axis: " << values.rightTriggerAxis);
   }

   // Buttons
   if (values.dPadLeft) {
      LOG_DEBUG("D-pad left");
   }
   if (values.dPadRight) {
      LOG_DEBUG("D-pad right");
   }
   if (values.dPadUp) {
      LOG_DEBUG("D-pad up");
   }
   if (values.dPadDown) {
      LOG_DEBUG("D-pad down");
   }

   if (values.faceLeft) {
      LOG_DEBUG("Left face");
   }
   if (values.faceRight) {
      LOG_DEBUG("Right face");
   }
   if (values.faceUp) {
      LOG_DEBUG("Up face");
   }
   if (values.faceDown) {
      LOG_DEBUG("Down face");
   }

   if (values.leftTrigger) {
      LOG_DEBUG("Left trigger");
   }
   if (values.rightTrigger) {
      LOG_DEBUG("Right trigger");
   }

   if (values.leftBumper) {
      LOG_DEBUG("Left bumper");
   }
   if (values.rightBumper) {
      LOG_DEBUG("Right bumper");
   }

   if (values.leftThumb) {
      LOG_DEBUG("Left thumb");
   }
   if (values.rightThumb) {
      LOG_DEBUG("Right thumb");
   }

   if (values.leftMenu) {
      LOG_DEBUG("Left menu");
   }
   if (values.rightMenu) {
      LOG_DEBUG("Right menu");
   }
}

void TestEngine::render() {
   glClear(GL_COLOR_BUFFER_BIT);
}

int main(int argc, char *argv[]) {
   Shiny::Result initResult = Shiny::init();
   if (initResult != Shiny::Result::kOK) {
      LOG_ERROR(Shiny::errorString(initResult));
      return EXIT_FAILURE;
   }

   int retVal = EXIT_SUCCESS;
   {
      TestEngine engine;

      Shiny::Result startResult = engine.startUp(100, 100, "Controller Tester");
      if (startResult == Shiny::Result::kOK) {
         engine.run();
      } else {
         LOG_ERROR(Shiny::errorString(startResult));
         retVal = EXIT_FAILURE;
      }
   }

   Shiny::terminate();

   return retVal;
   return 0;
}