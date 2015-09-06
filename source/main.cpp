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