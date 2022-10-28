#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/CinderImGui.h"

using namespace ci;
using namespace ci::app;
namespace gui = ImGui;

// We'll create a new Cinder Application by deriving from the App class.

struct mRect {
	glm::vec2 loc;
	glm::vec2 size;
	float rot = 0.f;
	bool rotate = false;
	mRect() : loc({ 100.f, 100.f }), size({ 100.f, 100.f }) {}
};

class BasicApp : public App {
  public:
	// Cinder will call 'mouseDrag' when the user moves the mouse while holding one of its buttons.
	// See also: mouseMove, mouseDown, mouseUp and mouseWheel.
	void mouseDrag( MouseEvent event ) override;

	// Cinder will call 'keyDown' when the user presses a key on the keyboard.
	// See also: keyUp.
	void keyDown( KeyEvent event ) override;

	// Cinder will call 'draw' each time the contents of the window need to be redrawn.
	void draw() override;
	void update() override;
	void setup() override;

	mRect rectangle;

  private:
	// This will maintain a list of points which we will draw line segments between
	std::vector<vec2> mPoints;
};

void prepareSettings( BasicApp::Settings* settings )
{
	settings->setMultiTouchEnabled( false );
}

void BasicApp::mouseDrag( MouseEvent event )
{
	// Store the current mouse position in the list.
	mPoints.push_back( event.getPos() );
}

void BasicApp::keyDown( KeyEvent event )
{
	if( event.getChar() == 'f' ) {
		// Toggle full screen when the user presses the 'f' key.
		setFullScreen( ! isFullScreen() );
	}
	else if( event.getCode() == KeyEvent::KEY_SPACE ) {
		// Clear the list of points when the user presses the space bar.
		mPoints.clear();
	}
	else if( event.getCode() == KeyEvent::KEY_ESCAPE ) {
		// Exit full screen, or quit the application, when the user presses the ESC key.
		if( isFullScreen() )
			setFullScreen( false );
		else
			quit();
	}
}

void BasicApp::setup() {
	ImGui::Initialize();
	float w = 0.87388417790257411;
	float x = -0.13669943685674510;
	float y = 0.35773628351431103;
	float z = -0.29944024283980192;

	glm::vec3 euler = glm::eulerAngles(glm::quat(w, x, z, y));
	euler = glm::degrees(euler);
	float yc = euler.z;
	euler.z = euler.y;
	euler.y = yc;
	euler.x++;

	ci::app::setFrameRate(30.f);

}

void BasicApp::update() {
	if (rectangle.rotate) {
		rectangle.rot += 0.01f;
	}
}

void BasicApp::draw()
{
	// Clear the contents of the window. This call will clear
	// both the color and depth buffers. 
	gl::clear( Color::gray( 0.1f ) );

	// Set the current draw color to orange by setting values for
	// red, green and blue directly. Values range from 0 to 1.
	// See also: gl::ScopedColor
	gl::color( 1.0f, 0.5f, 0.25f );

	// We're going to draw a line through all the points in the list
	// using a few convenience functions: 'begin' will tell OpenGL to
	// start constructing a line strip, 'vertex' will add a point to the
	// line strip and 'end' will execute the draw calls on the GPU.
	gl::begin( GL_LINE_STRIP );
	for( const vec2 &point : mPoints ) {
		gl::vertex( point );
	}
	gl::end();

	ImGui::ShowDemoWindow();

	{
		
		static ImVec2 mPos(0.f, 100.f);
		gui::SetNextWindowPos(mPos);

		gui::ScopedWindow window("sett");
		gui::DragFloat2("SIZE", &rectangle.size);
		gui::DragFloat2("LOC", &rectangle.loc);
		gui::DragFloat("ROT", &rectangle.rot, 0.01f);
		gui::Separator();
		gui::Checkbox("Auto-Rotate", &rectangle.rotate);

		mPos.x += 1.f;
		if (mPos.x > 1000)
			mPos.x = 0;

		
	}

	{
		ci::Rectf rect(-rectangle.size.x * .5f, -rectangle.size.y * .5f, rectangle.size.x * .5f, rectangle.size.y * .5f);
		ci::gl::ScopedColor clr(ci::Color::gray(.5f));
		ci::gl::ScopedModelMatrix mat;
		ci::gl::translate(rectangle.loc);
		gl::rotate(rectangle.rot);
		ci::gl::drawSolidRect(rect);
	}

}

// This line tells Cinder to actually create and run the application.
CINDER_APP( BasicApp, RendererGl, prepareSettings )
