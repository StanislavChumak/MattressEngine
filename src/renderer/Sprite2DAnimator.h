#ifndef SPRITE2DANIMATOR_H
#define SPRITE2DANIMATOR_H

namespace RenderEngine
{
    class Sprite2D;

    class Sprite2DAnimator
    {
    public:
        Sprite2DAnimator(Sprite2D *const sprite2D);
        Sprite2DAnimator(const Sprite2DAnimator &) = default;
        Sprite2DAnimator(Sprite2DAnimator &&) = default;
        Sprite2DAnimator &operator=(const Sprite2DAnimator &) = default;
        Sprite2DAnimator &operator=(Sprite2DAnimator &&) = default;
        ~Sprite2DAnimator();

        unsigned int get_current_frame() const { return _currentFrame; }
        double get_total_duration() const { return _totalDuration; }
        
        void update(const double &delta);
        void reset();

    private:
        Sprite2D *_sprite2D;
        unsigned int _currentFrame;
        double _currentFrameDuration;
        double _currentAnimationTime;
        double _totalDuration;
    };
}

#endif