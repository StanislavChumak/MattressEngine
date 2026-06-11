#ifndef PRESETS_HPP
#define PRESETS_HPP

#include "comp/registerComponent.hpp"
#include "sys/SystemManager.hpp"

#ifdef INCLUDE_CORE_COMPONENTS
#include "comp/core/Name.hpp"
#include "comp/core/ParentChildren.hpp"
#include "comp/core/Transform.hpp"

#include "sys/transform/GlobalTransformSystem.hpp"
#endif

#define REGISTER_CORE_COMPONENTS(core) \
REGISTER_COMPONENT(Name); \
REGISTER_COMPONENT(Parent); \
REGISTER_COMPONENT(Children); \
REGISTER_COMPONENT(Transform); \
core.systems.register_update<GlobalTransformSystem>(SystemPriority::TRANSFORM); \
core.systems.register_always_update<GlobalTransformSystem>()

#ifdef INCLUDE_SPRITE
#include "comp/single/Camera.hpp"
#include "comp/rendering/Sprite.hpp"

#include "sys/rendering/CameraSystem.hpp"
#include "sys/rendering/SpriteRenderSystem.hpp"
#endif

#define REGISTER_SPRITE(core) \
REGISTER_COMPONENT(Sprite); \
core.systems.register_update<CameraSystem>(SystemPriority::UI_LOGIC); \
core.systems.register_always_update<CameraSystem>(); \
core.systems.register_update<SpriteRenderSystem>(SystemPriority::RENDERING); \
core.systems.register_always_update<SpriteRenderSystem>()

#ifdef INCLUDE_ANIMATION
#include "comp/rendering/Animator.hpp"
#include "comp/rendering/StateAnimator.hpp"

#include "sys/rendering/AnimatorSystem.hpp"
#include "sys/rendering/StateAnimatorSystem.hpp"
#endif

#define REGISTER_ANIMATION(core) \
REGISTER_COMPONENT(Animator); \
REGISTER_COMPONENT(StateAnimator); \
core.systems.register_update<AnimatorSystem>(SystemPriority::ANIMATION); \
core.systems.register_always_update<AnimatorSystem>(); \
core.systems.register_update<StateAnimatorSystem>(SystemPriority::ANIMATION); \
core.systems.register_always_update<StateAnimatorSystem>()

#ifdef INCLUDE_SOUNDS
#include "comp/single/Audio.hpp"
#include "comp/audio/Sound.hpp"
#include "comp/audio/Music.hpp"

#include "sys/audio/AudioSystem.hpp"
#endif

#define REGISTER_SOUNDS(core) \
REGISTER_COMPONENT(Sound); \
REGISTER_COMPONENT(Music); \
core.world.add_single_comp(Audio()); \
core.systems.register_update<AudioSystem>(SystemPriority::AUDIO); \
core.systems.register_always_update<AudioSystem>()

#ifdef INCLUDE_LOCATION_SOUNDS
#include "comp/single/Listener.hpp"

#include "sys/audio/LocationAudioSystem.hpp"
#endif

#define REGISTER_LOCATION_SOUNDS(core) \
core.systems.register_update<LocationAudioSystem>(SystemPriority::AUDIO); \
core.systems.register_always_update<LocationAudioSystem>(); \
core.world.add_single_comp(Listener())

#endif
