#include "Transformable.hpp"

#include "Clock.hpp"

void Transformable::Delay(double duration)
{
    this->delay += duration;
}

void Transformable::Then()
{
    // return early if theres no added transforms, as there would be none to delay from
    if (transforms.empty())
        return;

    // get the duration of the last added transform and use it as the delay
    Delay(transforms.back().EndTime - Clock::GetTime());
}

void Transformable::BeginSequence()
{
    in_sequence = true;
}

void Transformable::EndSequence()
{
    in_sequence = false;
    delay = 0;
}

void Transformable::AddTransform(double duration,
                                 float *start_value,
                                 float end_value,
                                 Easing easing,
                                 std::function<void(float)> callback)
{
    // add the new transform
    double t = Clock::GetTime();
    transforms.push_back((Transform)
    {
        .StartTime = t + delay,
        .EndTime = t + delay + duration,
        .Started = false,
        .StartValuePointer = start_value,
        .StartValue = 0,
        .EndValue = end_value,
        .EasingType = easing,
        .Callback = callback,
    });

    // reset the delay
    if (!in_group && !in_sequence)
        delay = 0;
}

void Transformable::BeginGroup()
{
    in_group = true;
}

void Transformable::EndGroup()
{
    in_group = false;
    if (!in_sequence)
        delay = 0;
}

void Transformable::UpdateTransforms()
{
    // get the current time
    double c = Clock::GetTime();

    // for each transform
    for (auto t = transforms.begin(); t < transforms.end(); t++)
    {
        // dont bother if the transform starts after the current time
        if (c < t->StartTime)
            continue;

        // start the transform if it hasnt been yet
        if (!t->Started)
        {
            t->StartValue = *t->StartValuePointer;
            t->Started = true;
        }

        // call the final interpolate and remove the current tranform if it is finished
        if (c > t->EndTime)
        {
            t->Callback(interpolate(&(*t), t->EndTime));
            transforms.erase(t);
            continue;
        }

        // interpolate the current transform
        t->Callback(interpolate(&(*t), c));
    }
}

double Transformable::interpolate(Transform *transform, double time)
{
    // get the values used for interpolating
    // t = time
    // b = begin
    // c = change
    // d = duration
    double t = time - transform->StartTime;
    float b = transform->StartValue;
    float c = transform->EndValue - transform->StartValue;
    double d = transform->EndTime - transform->StartTime;

    // interpolate with the given easing
    switch (transform->EasingType)
    {
        case Easing::None:
            return c * t / d + b;
        case Easing::InQuad:
            return c * (t /= d) * t + b;
        case Easing::OutQuad:
            return -c * (t /= d) * (t - 2) + b;
    }
}
