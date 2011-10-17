#ifndef TELEMIDIALINKS_H_
#define TELEMIDIALINKS_H_

#ifdef __cplusplus
extern "C" {
#endif

void browserShow(struct session* ses);
void browserHide(struct session* ses);
void browserResizeCoord(
		struct session* resizeSession,
		double cx, double cy, double cw, double ch);

void browserSetFocusHandler(int isHandler, struct session* someSes);
void browserReceiveEvent(struct session* ses, void* event);
void* getDFBWindow(struct session* someSes);
void* browserGetSurface(struct session* someSes);
void browserSetFlipWindow(struct session* ses, void* flipWindow);
void setBrowserDFB(void* dfb);
void browserSetAlpha(int alpha, struct session* ses);
void browserRequestFocus(struct session* ses);
void setDisplayMenu(int toDisplay);
struct session* openBrowser(double cx, double cy, double cw, double ch);
void loadUrlOn(struct session* ses, const char* url);
void setGhostBrowser(struct session* ses);
int closeBrowser(struct session* ses);
int destroyAllBrowsers();

#ifdef __cplusplus
}
#endif

#endif /*TELEMIDIALINKS_H_*/

