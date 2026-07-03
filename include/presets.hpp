#ifndef PRESETS_HPP
#define PRESETS_HPP

#include "sys/SystemManager.hpp"

#ifdef INCLUDE_CORE_COMPONENTS
#include "comp/core/Script.hpp"
#include "comp/core/Transform.hpp"

#include "sys/core/InputSystem.hpp"
#include "sys/core/ScriptsSystem.hpp"
#include "sys/core/UpdateTransformSystem.hpp"
#endif

#define REGISTER_CORE_COMPONENTS(core) \
core.systems.register_update<mtrs::sys::InputSystem>(); \
core.systems.register_always_update<mtrs::sys::InputSystem>(); \
core.systems.register_update<mtrs::sys::ScriptsSystem>(); \
core.systems.register_always_update<mtrs::sys::ScriptsSystem>(); \
core.systems.register_update<mtrs::sys::UpdateTransformSystem>(); \
core.systems.register_always_update<mtrs::sys::UpdateTransformSystem>()


#ifdef INCLUDE_SPRITE
#include "comp/single/Camera.hpp"
#include "comp/rendering/Sprite.hpp"

#include "sys/rendering/CameraSystem.hpp"
#include "sys/rendering/SpriteRenderSystem.hpp"
#endif

#define REGISTER_SPRITE(core) \
core.systems.register_update<mtrs::sys::CameraSystem>(); \
core.systems.register_always_update<mtrs::sys::CameraSystem>(); \
core.systems.register_update<mtrs::sys::SpriteRenderSystem>(); \
core.systems.register_always_update<mtrs::sys::SpriteRenderSystem>()

#ifdef INCLUDE_ANIMATION
#include "comp/rendering/Animator.hpp"
#include "comp/rendering/StateAnimator.hpp"

#include "sys/rendering/AnimatorSystem.hpp"
#include "sys/rendering/StateAnimatorSystem.hpp"
#endif

#define REGISTER_ANIMATION(core) \
core.systems.register_update<mtrs::sys::AnimatorSystem>(); \
core.systems.register_always_update<mtrs::sys::AnimatorSystem>(); \
core.systems.register_update<mtrs::sys::StateAnimatorSystem>(); \
core.systems.register_always_update<mtrs::sys::StateAnimatorSystem>()

#ifdef INCLUDE_SPRITE_MAP
#include "comp/rendering/Sprite.hpp"
#include "comp/rendering/MapAnimator.hpp"
#include "sys/rendering/SpriteMapRenderSystem.hpp"
#include "sys/rendering/MapAnimatorSystem.hpp"
#endif

#define REGISTER_SPRITE_MAP(core) \
core.systems.register_update<mtrs::sys::SpriteMapRenderSystem>();\
core.systems.register_always_update<mtrs::sys::SpriteMapRenderSystem>();\
core.systems.register_update<mtrs::sys::MapAnimatorSystem>();\
core.systems.register_always_update<mtrs::sys::MapAnimatorSystem>()

#ifdef INCLUDE_SOUNDS
#include "comp/single/Audio.hpp"
#include "comp/audio/SoundPlayer.hpp"
#include "res/asset/Sound.hpp"
#endif

#define REGISTER_SOUNDS(core) 

#ifdef INCLUDE_LOCATION_SOUNDS
#include "comp/single/Listener.hpp"

#include "sys/audio/LocationAudioSystem.hpp"
#endif

#define REGISTER_LOCATION_SOUNDS(core) \
core.systems.register_update<mtrs::sys::LocationAudioSystem>(); \
core.systems.register_always_update<mtrs::sys::LocationAudioSystem>();

#endif
