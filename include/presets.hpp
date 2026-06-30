#ifndef PRESETS_HPP
#define PRESETS_HPP

#include "sys/SystemManager.hpp"

#ifdef INCLUDE_CORE_COMPONENTS
#include "comp/core/Name.hpp"
#include "comp/core/ParentChildren.hpp"
#include "comp/core/Transform.hpp"

#include "sys/transform/GlobalTransformSystem.hpp"
#endif

#define REGISTER_CORE_COMPONENTS(core) \
core.systems.register_update<mtrs::sys::GlobalTransformSystem>(); \
core.systems.register_always_update<mtrs::sys::GlobalTransformSystem>()

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

#ifdef INCLUDE_SOUNDS
#include "comp/single/Audio.hpp"
#include "comp/audio/SoundPlayer.hpp"
#include "res/asset/Sound.hpp"
#endif

#define REGISTER_SOUNDS(core) \
core.systems.register_update<mtrs::sys::AudioSystem>(); \
core.systems.register_always_update<mtrs::sys::AudioSystem>()

#ifdef INCLUDE_LOCATION_SOUNDS
#include "comp/single/Listener.hpp"

#include "sys/audio/LocationAudioSystem.hpp"
#endif

#define REGISTER_LOCATION_SOUNDS(core) \
core.systems.register_update<mtrs::sys::LocationAudioSystem>(); \
core.systems.register_always_update<mtrs::sys::LocationAudioSystem>();

#endif
