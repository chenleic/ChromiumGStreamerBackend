description("Tests that Geolocation correctly handles multiple concurrent requests.");

var mockLatitude = 51.478;
var mockLongitude = -0.166;
var mockAccuracy = 100;

if (!window.testRunner || !window.internals)
    debug('This test can not run without testRunner or internals');

internals.setGeolocationClientMock(document);
internals.setGeolocationPermission(document, true);
internals.setGeolocationPosition(document, mockLatitude, mockLongitude, mockAccuracy);

var watchCallbackInvoked = false;
var oneShotCallbackInvoked = false;

navigator.geolocation.watchPosition(function(p) {
    shouldBeFalse('watchCallbackInvoked');
    watchCallbackInvoked = true;
    maybeFinishTest(p);
}, function() {
    testFailed('Error callback invoked unexpectedly');
    finishJSTest();
});

navigator.geolocation.getCurrentPosition(function(p) {
    shouldBeFalse('oneShotCallbackInvoked');
    oneShotCallbackInvoked = true;
    maybeFinishTest(p);
}, function() {
    testFailed('Error callback invoked unexpectedly');
    finishJSTest();
});

var position;
function maybeFinishTest(p) {
    position = p;
    shouldBe('position.coords.latitude', 'mockLatitude');
    shouldBe('position.coords.longitude', 'mockLongitude');
    shouldBe('position.coords.accuracy', 'mockAccuracy');
    if (watchCallbackInvoked && oneShotCallbackInvoked)
        finishJSTest();
}

window.jsTestIsAsync = true;
