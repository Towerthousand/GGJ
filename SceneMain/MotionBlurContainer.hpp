#ifndef MOTIONBLURCONTAINER_HPP
#define MOTIONBLURCONTAINER_HPP
#include "commons.hpp"

class MotionBlurContainer : public ContainerObject {
	public:
		MotionBlurContainer();
		~MotionBlurContainer();

		virtual void draw() const;

		mutable RenderTarget* target1;
		mutable RenderTarget* target2;
		mutable Model quad;
	private:
};

#endif // MOTIONBLURCONTAINER_HPP
