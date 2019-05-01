#pragma once

#include <vector>
#include <functional>

// the different easing modes for a transform interpolation
enum Easing
{
    None,
    InQuad,
    OutQuad,
};

// a base class to inherit from to add interpolating transform functionality to a class
class Transformable
{
    private:
        // a currently active transform on a transformable
        struct Transform
        {
            // the start and end times of the interpolation of this transform
            double StartTime, EndTime;

            // whether or not this transform has started interpolating
            // set to true when time >= StartTime
            bool Started;

            // a pointer to where to get the start value from when this transform starts
            // when this transform is started, the value at this pointer is copied to StartValue
            // and used as the start value to interpolate to EndValue
            float *StartValuePointer;

            // the values for this transform to interpolate between
            float StartValue, EndValue;

            // the easing for this transform to interpolate with
            Easing EasingType;

            // the function for this transform to call when it is updated
            // passed the current interpolated value between StartValue and EndValue
            std::function<void(float)> Callback;
        };

        // all the current transforms in this transformable
        std::vector<Transform> transforms;

        // the current delay to add to new transforms
        // resets to zero when used and not in a group/sequence
        double delay = 0;

        // whether or not this transformable is currently adding a group/sequence of transforms
        bool in_group = false, in_sequence = false;

        // return the interpolated value of the given transform at the given time
        // note: uses transform->StartValue, so ensure it is set before calling
        double interpolate(Transform *transform, double time);

    protected:
        // add a new transform to this transformable
        // uses the current time as the start time, adding the given duration to get the end time
        // interpolates between the given start and end values with the given easing
        // calls the given callback when the current interpolated value is updated
        // note: start_value is a pointer so the transform can copy the value when it starts, not when its added
        void AddTransform(double duration,
                          float *start_value,
                          float end_value,
                          Easing easing,
                          std::function<void(float)> callback);

        // begin/end a group of transforms that should all use the same current delay
        // used for things like types with multiple properties
        // when EndGroup() is called and a sequence is not currently being added the delay is reset to zero
        void BeginGroup();
        void EndGroup();

        // update all the current transforms of this transformable
        void UpdateTransforms();

    public:
        // delay the next added transform for the given duration
        // if multiple delays are called without adding transforms then the delay is increased
        void Delay(double duration);

        // delay the next added transform with the relative end time of the last added transform
        void Then();

        // begin/end a sequence of transforms that should all use the same current delay
        // when EndSequence() is called then the delay is reset to zero
        void BeginSequence();
        void EndSequence();
};
