#include "main.hpp"
#include "GlobalNamespace/OculusVRHelper.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/XR/XRNode.hpp"

MAKE_HOOK_MATCH(
    AdjustControllerTransform, 
    &GlobalNamespace::OculusVRHelper::AdjustControllerTransform, 
    void, 
    GlobalNamespace::OculusVRHelper* self,
    UnityEngine::XR::XRNode node, 
    UnityEngine::Transform* transform, 
    UnityEngine::Vector3 position, 
    UnityEngine::Vector3 rotation
    ) 
{
    if(node == UnityEngine::XR::XRNode::LeftHand) { //test for left hand
        rotation.z = -rotation.z; //fixes rotation
    }
    AdjustControllerTransform(self, node, transform, position, rotation);
}

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup

// Loads the config from disk using our modInfo, then returns it for use
Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
	
    getConfig().Load(); // Load the config file
    getLogger().info("Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();

    getLogger().info("Installing hooks...");
    INSTALL_HOOK(getLogger(), AdjustControllerTransform);
    getLogger().info("Installed all hooks!");
}