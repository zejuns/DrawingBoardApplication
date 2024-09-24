
#include "Core.h"
#include "App.h"

App* app;

int main()
{
    app = new App();
    app->Init();
    return app->Run();
}
