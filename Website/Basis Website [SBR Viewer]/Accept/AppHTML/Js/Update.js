
function findAndReplace(searchText, replacement, searchNode) {
    if (!searchText || typeof replacement === 'undefined') {
        // Throw error here if you want...
        return;
    }
    var regex = typeof searchText === 'string' ?
                new RegExp(searchText, 'g') : searchText,
        childNodes = (searchNode || document.body).childNodes,
        cnLength = childNodes.length,
        excludes = 'html,head,style,title,link,script,object,iframe';   // meta, !!
    while (cnLength--) {
        var currentNode = childNodes[cnLength];
        if (currentNode.nodeType === 1 &&
            (excludes + ',').indexOf(currentNode.nodeName.toLowerCase() + ',') === -1) {
            arguments.callee(searchText, replacement, currentNode);
        }
        if (currentNode.nodeType !== 3 || !regex.test(currentNode.data)) {
            continue;
        }
        var parent = currentNode.parentNode,
            frag = (function () {
                var html = currentNode.data.replace(regex, replacement),
                    wrap = document.createElement('div'),
                    frag = document.createDocumentFragment();
                wrap.innerHTML = html;
                while (wrap.firstChild) {
                    frag.appendChild(wrap.firstChild);
                }
                return frag;
            })();
        parent.insertBefore(frag, currentNode);
        parent.removeChild(currentNode);
    }
}
function InitPage(bSHinit,sAFVersie, sAFBuild, sRPVersie, sRPRevisie, sAFVWrd, sAFBWrd, sRPVWrd, sRPRWrd) {
    var sLstVersieStart = "<a href='TabPage.asp?pageid=";
    var sLstVersieStop = "'>laatste versie " + sAFVWrd + " (build " + sAFBWrd + ") - rapportage " + sRPVWrd + sRPRWrd + "    &#187;</a>";
    findAndReplace(sAFVersie, sAFVWrd);
    findAndReplace(sAFBuild,sAFBWrd);
    findAndReplace(sRPVersie,sRPVWrd);
    findAndReplace(sRPRevisie, sRPRWrd);
    findAndReplace("@LstVersieStop@", sLstVersieStop);
    findAndReplace("@LstVersieStart@", sLstVersieStart);
    if (bSHinit==1) initShowHide();
    highlight();
}