ofxTemplateFace
===============

attempt to use opencv cvMatchTemplate for a fast face detection in openFrameworks
this is far away from usable, but shows how perfomant the cvMatchTemplate is
it scans every x frame for a face with detectMultiScale and uses this as an template for cvMatchTemplate

there are many ways to improve performance, like
- search for the face template in a smaller window
- only scan every x frames for new faces to add them to a template queue
- do not replace every frame the old facetemplate, but keep a queue of positive templates (now it tracks everything after a while)
- what about the face size ? no way to get this running only using cvMatchtemplate
- use additionaly color tone tracking (blobtracking) http://bsd-noobz.com/opencv-guide/60-5-skin-detection-for-fast-face-tracking
- even better: implement something like haar + camshift https://gist.github.com/lamberta/231696