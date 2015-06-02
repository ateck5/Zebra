// JScript source code
var active_div_id_sel = new Array();
var active_div_id = new Array();
var selAll = 0;

function isEmpty(ob) {
    if (!ob || ob.length == 0) {
        return true;
    }
    // checks for a non-white space character 
    // which I think [citation needed] is faster 
    // than removing all the whitespace and checking 
    // against an empty string
    return !/[^\s]+/.test(ob);
}
function hasClass(ele, cls) {
    return ele.className.match(new RegExp('(\\s|^)' + cls + '(\\s|$)'));
}

function addClass(ele, cls) {
    if (!this.hasClass(ele, cls)) ele.className += " " + cls;
}

function removeClass(ele, cls) {
    if (hasClass(ele, cls)) {
        var reg = new RegExp('(\\s|^)' + cls + '(\\s|$)');
        ele.className = ele.className.replace(reg, ' ');
    }
}
function HideContent(d) {
    if (isEmpty(d)) { return; }
    if (d.length < 1) { return; }
    document.getElementById(d).style.display = "none";
}
function ShowContent(d,l) {
    if (isEmpty(d)) { return; }
    if (d.length < 1) { return; }
    HideContent(active_div_id[l]);  // Hide active id.
    document.getElementById(d).style.display = "block";
    active_div_id[l] = d;  // New active id.
}
function HideBgImg(s,l) {
    if (isEmpty(s)) { return; }
    if (s.length < 1) { return; }
    document.getElementById(s).style.backgroundImage = "url(images/zoomplus.gif)";
    removeClass(document.getElementById(s), "open")
    active_div_id_sel[l] = s;
}
function ShowBgImg(s,l) {
    if (isEmpty(s)) { return; }
    if (s.length < 1) { return; }
    HideBgImg(active_div_id_sel[l],l);
    document.getElementById(s).style.backgroundImage = "url(images/zoomminus.gif)";
    addClass(document.getElementById(s), "open");
    active_div_id_sel[l] = s;
}
function ReverseContentDisplay(d,s,l) {
    if ((d.length < 1) || (s.length < 1)) { return; }
    if (document.getElementById(d).style.display == "none") {
        ShowContent(d,l);  // Code changed here!!!
        ShowBgImg(s,l);  
    }
    else {
        HideContent(d); // Code changed here!!!
        HideBgImg(s,l);
    }
    $("div[class='error']").each(function (index) { this.style.display = 'none'; });
    $("span[class='error']").each(function (index) { this.style.display = 'none'; });
}
function openShowHide(node) {
    var parent = node;
    while (parent && (parent.className != "showhidegroup")) {
        parent = parent.parentNode;
    }
    if (parent && (parent.className == "showhidegroup")) {
        if (parent.style.display == "none") {
            parent.style.display = "block"
            var s = ("S" + parent.id);
            document.getElementById(s).style.backgroundImage = "url(images/zoomminus.gif)";
            addClass(document.getElementById(s), "open");
            if (parent.getAttribute("level") == 2) openShowHide(parent.parentNode);
        }

    }
}
function closeAllShowHide() {
    HideContent(active_div_id[1]);  
    HideContent(active_div_id[2]);  
    HideBgImg(active_div_id_sel[1], 1);
    HideBgImg(active_div_id_sel[2], 2);
}
function toggleAllShowHide(s) {
    if (selAll == 0) {
        selAll = 1;
        document.getElementById(s).innerHTML = "Verberg alles";
    }
    else {
        selAll = 0;
        document.getElementById(s).innerHTML = "Toon alles";
    }
    for (var x = 1; x <= 100; x++) {
        var el = document.getElementById("SH" + x);
        if (isEmpty(el)) { return; }
        if (selAll == 1) { el.style.display = "block"; } else { el.style.display = "none"; }

        el = document.getElementById("SSH" + x);
        if (isEmpty(el)) { return; }
        if (selAll == 1) {
            el.style.backgroundImage = "url(images/zoomminus.gif)"; 
            addClass(el, "open");
        } else {
            el.style.backgroundImage = "url(images/zoomplus.gif)";
            removeClass(el, "open");
        }
    }
}
function initShowHide() {
    active_div_id_sel[1] = 'SSH1';
    active_div_id[1] = 'SH1';
}
