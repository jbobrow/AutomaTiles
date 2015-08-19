//	Thanks Dinesh Ygv
//	http://stackoverflow.com/users/1439313/dinesh-ygv

var blobObject = null;

function createDownloadLink(anchorSelector, str, fileName){

    var element = document.getElementById(anchorSelector);

    if(window.navigator.msSaveOrOpenBlob)
    {
        var fileData = [str];
        blobObject = new Blob(fileData);
        element.addEventListener("click", function()
        {
            window.navigator.msSaveOrOpenBlob(blobObject, fileName);
        });
    }
    else
    {
        var container = document.querySelector('#two');
        var url = "data:image/svg+xml," + container.innerHTML;
        element.setAttribute("href", url);
    }
}

document.addEventListener("click", function(){
    var str = document.getElementById("two").innerHTML;
    var filename = "automatiles" + Date.now()%1000000 + ".svg";
    createDownloadLink("save",str,filename);
});

var filename = "automatiles" + Date.now()%1000000 + ".svg";
document.getElementById("save").download = filename;