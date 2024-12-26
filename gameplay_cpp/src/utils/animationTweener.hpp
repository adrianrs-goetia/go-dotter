#pragma once

#include <core/core.hpp>
#include <godot_cpp/classes/animation_mixer.hpp>
#include <stack>

/* The animationTweener is a class that handles tweening between animations.
* It keeps a record of a set of jobs; the animation paramters it is currently tweening.
* for each process step it tweens the parameters, and removes the jobs if the tweening is completed 
* 
* Currently all tweens are linear
*/
class AnimationTweener {
private:
    struct TweenJob{
        float desiredValue;
        float currentValue;
        float drive;
        // ? tweenType;
    };
    std::map<std::string, TweenJob> jobs;
    AnimationMixer& animator;

public:	
    AnimationTweener(AnimationMixer& _animator) : animator(_animator) {};

    void process(float delta) {
        std::stack<std::string> toCleanUp;

        auto timestamp = std::chrono::system_clock::now();
        for (auto& tweenjob : jobs) {
            
            std::string parameter = tweenjob.first.c_str();
            TweenJob& job   = tweenjob.second;
            // drive current value towards desired
            float current = job.currentValue;
            float desired = job.desiredValue;
            float actualDrive = job.drive*delta;
            if (current>desired) {
                current-=actualDrive;
                if (current<=desired) {
                    current=desired;
                    toCleanUp.push(tweenjob.first);
                }
            } else if (current<desired) {
                current+=actualDrive;
                if (current>=desired) {
                    current=desired;
                    toCleanUp.push(tweenjob.first);
                }
            } else {
                toCleanUp.push(tweenjob.first);
            }
            animator.set(parameter.c_str(), current);
            job.currentValue=current;
        }

        while(!toCleanUp.empty()) {
            jobs.erase(toCleanUp.top());
            toCleanUp.pop();
        }
    }

    void tween(std::string parameter, float desiredValue, float drive) {
        float originalValue = animator.get(parameter.c_str());
        jobs.insert_or_assign(parameter, TweenJob{desiredValue, originalValue, drive});
    }
};