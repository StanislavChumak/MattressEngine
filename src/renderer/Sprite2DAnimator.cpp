#include "Sprite2DAnimator.h"

#include "Sprite2D.h"

namespace RenderEngine
{
    Sprite2DAnimator::Sprite2DAnimator(Sprite2D *const sprite2D)
    : _sprite2D(std::move(sprite2D))
    , _currentFrame(0)
    , _currentFrameDuration(_sprite2D->get_frame_duration(0))
    , _currentAnimationTime(0)
    , _totalDuration(0)
    {
        for(unsigned int currentFrameId = 0; currentFrameId < _sprite2D->get_frames_count(); ++currentFrameId)
        {
            _totalDuration += _sprite2D->get_frame_duration(currentFrameId);
        }
    }

    Sprite2DAnimator::~Sprite2DAnimator()
    {
    }

    void Sprite2DAnimator::update(const double &delta)
    {

        _currentAnimationTime += delta;

        while (_currentAnimationTime >= _currentFrameDuration)
        {
            _currentAnimationTime -= _currentFrameDuration;
            ++_currentFrame;

            if (_currentFrame == _sprite2D->get_frames_count())
            {
                _currentFrame = 0;
            }
            _currentFrameDuration = _sprite2D->get_frame_duration(_currentFrame);
        }
    }

    void Sprite2DAnimator::reset()
    {
        _currentFrame = 0;
        _currentFrameDuration = _sprite2D->get_frame_duration(0);
        _currentAnimationTime = 0;
    }
}