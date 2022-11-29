#pragma once

#include "../JsonForward.h"
#include <Interfaces/ClientInterfaces.h>
#include <Interfaces/OtherInterfaces.h>
#include <Platform/IsPlatform.h>
#include <Config/ResetConfigurator.h>
#include "Visuals/ColorCorrection.h"
#include "Visuals/SkyboxChanger.h"

namespace csgo { enum class FrameStage; }
class GameEvent;
struct ImDrawList;
class EngineInterfaces;

class Visuals {
public:
    Visuals(const Memory& memory, OtherInterfaces interfaces, ClientInterfaces clientInterfaces, EngineInterfaces engineInterfaces, const helpers::PatternFinder& clientPatternFinder, const helpers::PatternFinder& enginePatternFinder)
        : memory{ memory }, interfaces{ interfaces }, clientInterfaces{ clientInterfaces }, engineInterfaces{ engineInterfaces }, skyboxChanger{ createSkyboxChanger(interfaces.getCvar(), enginePatternFinder) }
    {
#if IS_WIN32()
        disablePostProcessingPtr = reinterpret_cast<bool*>(clientPatternFinder("\x83\xEC\x4C\x80\x3D").add(5).deref().get());
#elif IS_LINUX()
        disablePostProcessingPtr = reinterpret_cast<bool*>(clientPatternFinder("\x80\x3D?????\x89\xB5").add(2).relativeToAbsolute().get());
#endif
    }

    bool isThirdpersonOn() noexcept;
    bool isZoomOn() noexcept;
    bool isSmokeWireframe() noexcept;
    bool isDeagleSpinnerOn() noexcept;
    bool shouldRemoveFog() noexcept;
    bool shouldRemoveScopeOverlay() noexcept;
    bool shouldRemoveSmoke() noexcept;
    float viewModelFov() noexcept;
    float fov() noexcept;
    float farZ() noexcept;

    void performColorCorrection() noexcept;
    void inverseRagdollGravity() noexcept;
    void colorWorld() noexcept;
    void modifySmoke(csgo::FrameStage stage) noexcept;
    void thirdperson() noexcept;
    void removeVisualRecoil(csgo::FrameStage stage) noexcept;
    void removeBlur(csgo::FrameStage stage) noexcept;
    void updateBrightness() noexcept;
    void removeGrass(csgo::FrameStage stage) noexcept;
    void remove3dSky() noexcept;
    void removeShadows() noexcept;
    void applyZoom(csgo::FrameStage stage) noexcept;
    void applyScreenEffects() noexcept;
    void hitEffect(const GameEvent* event = nullptr) noexcept;
    void hitMarker(const GameEvent* event, ImDrawList* drawList = nullptr) noexcept;
    void disablePostProcessing(csgo::FrameStage stage) noexcept;
    void reduceFlashEffect() noexcept;
    bool removeHands(const char* modelName) noexcept;
    bool removeSleeves(const char* modelName) noexcept;
    bool removeWeapons(const char* modelName) noexcept;
    void skybox(csgo::FrameStage stage) noexcept;
    void bulletTracer(const GameEvent& event) noexcept;
    void drawMolotovHull(ImDrawList* drawList) noexcept;

    void updateEventListeners(bool forceRemove = false) noexcept;
    void updateInput() noexcept;

    // GUI
    void menuBarItem() noexcept;
    void tabItem() noexcept;
    void drawGUI(bool contentOnly) noexcept;

    // Config
    json toJson() noexcept;
    void fromJson(const json& j) noexcept;
    void resetConfig() noexcept;

private:
    const Memory& memory;
    OtherInterfaces interfaces;
    ClientInterfaces clientInterfaces;
    EngineInterfaces engineInterfaces;
    bool* disablePostProcessingPtr;
    ColorCorrection colorCorrection;
    SkyboxChanger skyboxChanger;
};
