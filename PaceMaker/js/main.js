/**
 * Created by jonathanbobrow on 10/9/15.
 */
var isActive = false;

// main function (on load)
$(function () {
    init();
});

var loadJSON = function (callback) {

    var xobj = new XMLHttpRequest();
    xobj.overrideMimeType("application/json");
    xobj.open('GET', 'data/data.json', true); // Replace 'my_data' with the path to your file
    xobj.onreadystatechange = function () {
        if (xobj.readyState == 4 && xobj.status == "200") {
            // Required use of an anonymous callback as .open will NOT return a value but simply returns undefined in asynchronous mode
            callback(xobj.responseText);
        }
    };
    xobj.send(null);
};

var init = function () {
    loadJSON(function (response) {
        // Parse JSON string into object
        var data = JSON.parse(response);

        console.log("loading json");
        document.getElementById("outgoing_bpm").innerHTML = data.heartbeats.outgoing;
        document.getElementById("incoming_bpm").innerHTML = data.heartbeats.incoming;
        // set active state
        isActive = data.isActive;
    });
};

var toggleActive = function() {
    var button_text;

    isActive = !isActive;

    if(isActive)
        button_text = "make inactive";
    else
        button_text = "make active";

    document.getElementById("isActive").innerHTML = button_text;
    console.log("isActive" + isActive);
};
