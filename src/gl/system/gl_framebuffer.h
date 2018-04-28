#ifndef __GL_FRAMEBUFFER
#define __GL_FRAMEBUFFER

#include "gl_sysfb.h"

#include <memory>

class FHardwareTexture;
class FSimpleVertexBuffer;
class FGLDebug;

class OpenGLFrameBuffer : public SystemFrameBuffer
{
	typedef SystemFrameBuffer Super;

public:

	explicit OpenGLFrameBuffer() {}
	OpenGLFrameBuffer(void *hMonitor, int width, int height, int bits, int refreshHz, bool fullscreen) ;
	~OpenGLFrameBuffer();

	void InitializeState();
	void Update();

	// Color correction
	bool SetGamma (float gamma);
	bool SetBrightness(float bright);
	bool SetContrast(float contrast);
	void DoSetGamma();

	void CleanForRestart() override;
	void UpdatePalette() override;
	void GetFlashedPalette (PalEntry pal[256]) override;
	PalEntry *GetPalette () override;
	bool SetFlash(PalEntry rgb, int amount) override;
	void GetFlash(PalEntry &rgb, int &amount) override;
	void Begin2D(bool copy3d) override;
	void GameRestart() override;
	void InitForLevel() override;
	void SetClearColor(int color) override;
	uint32_t GetCaps() override;
	void RenderTextureView(FCanvasTexture *tex, AActor *Viewpoint, double FOV) override;
	void WriteSavePic(player_t *player, FileWriter *file, int width, int height) override;
	void RenderView(player_t *player) override;
	void SetTextureFilterMode() override;
	IHardwareTexture *CreateHardwareTexture(FTexture *tex) override;
	FModelRenderer *CreateModelRenderer(int mli) override;
	void UnbindTexUnit(int no) override;
	void FlushTextures() override;
	void TextureFilterChanged() override;
	void ResetFixedColormap() override;

	// Retrieves a buffer containing image data for a screenshot.
	// Hint: Pitch can be negative for upside-down images, in which case buffer
	// points to the last row in the buffer, which will be the first row output.
	virtual void GetScreenshotBuffer(const uint8_t *&buffer, int &pitch, ESSType &color_type, float &gamma) override;

	// Releases the screenshot buffer.
	virtual void ReleaseScreenshotBuffer();

	bool WipeStartScreen(int type);
	void WipeEndScreen();
	bool WipeDo(int ticks);
	void WipeCleanup();
	void Swap();
	bool IsHWGammaActive() const { return HWGammaActive; }

	void SetVSync(bool vsync);

	void ScaleCoordsFromWindow(int16_t &x, int16_t &y) override;
	void Draw2D() override;

	bool HWGammaActive = false;			// Are we using hardware or software gamma?
	std::shared_ptr<FGLDebug> mDebug;	// Debug API
private:
	PalEntry Flash;						// Only needed to support some cruft in the interface that only makes sense for the software renderer
	PalEntry SourcePalette[256];		// This is where unpaletted textures get their palette from
	uint8_t *ScreenshotBuffer;			// What the name says. This must be maintained because the software renderer can return a locked canvas surface which the caller cannot release.
	int camtexcount = 0;

	class Wiper
	{

	protected:
		FSimpleVertexBuffer *mVertexBuf;

		void MakeVBO(OpenGLFrameBuffer *fb);

	public:
		Wiper();
		virtual ~Wiper();
		virtual bool Run(int ticks, OpenGLFrameBuffer *fb) = 0;
	};

	class Wiper_Melt;			friend class Wiper_Melt;
	class Wiper_Burn;			friend class Wiper_Burn;
	class Wiper_Crossfade;		friend class Wiper_Crossfade;

	Wiper *ScreenWipe;
	FHardwareTexture *wipestartscreen;
	FHardwareTexture *wipeendscreen;


public:
};


#endif //__GL_FRAMEBUFFER
