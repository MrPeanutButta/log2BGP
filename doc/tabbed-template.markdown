<html>
<head>
<meta charset="utf-8">
<title>Pure Javascript, HTML 5 &amp; CSS3 Tabs</title>
<link href="tabs.css" rel="stylesheet" type="text/css">
</head>
<body>
<div id="wrapper">
  <div id="tabContainer">
    <div class="tabs">
      <ul>
        <li id="tabHeader_1">Page 1</li>
        <li id="tabHeader_2">Page 2</li>
        <li id="tabHeader_3">Page 3</li>
      </ul>
    </div>
    <div class="tabscontent">
      <div class="tabpage" id="tabpage_1">
        <h2>Page 1</h2>
        <p>Pellentesque habitant morbi tristique senectus...</p>
      </div>
      <div class="tabpage" id="tabpage_2">
        <h2>Page 2</h2>
        <p>Pellentesque habitant morbi tristique senectus...</p>
      </div>
      <div class="tabpage" id="tabpage_3">
        <h2>Page 3</h2>
        <p>Pellentesque habitant morbi tristique senectus...</p>
      </div>
    </div>
  </div>
<script>
window.onload=function() {

  // get tab container
  var container = document.getElementById("tabContainer");
    // set current tab
    var navitem = container.querySelector(".tabs ul li");
    //store which tab we are on
    var ident = navitem.id.split("_")[1];
    navitem.parentNode.setAttribute("data-current",ident);
    //set current tab with class of activetabheader
    navitem.setAttribute("class","tabActiveHeader");

    //hide two tab contents we don't need
    var pages = container.querySelectorAll(".tabpage");
    for (var i = 1; i < pages.length; i++) {
      pages[i].style.display="none";
    }

    //this adds click event to tabs
    var tabs = container.querySelectorAll(".tabs ul li");
    for (var i = 0; i < tabs.length; i++) {
      tabs[i].onclick=displayPage;
    }
}

// on click of one of tabs
function displayPage() {
  var current = this.parentNode.getAttribute("data-current");
  //remove class of activetabheader and hide old contents
  document.getElementById("tabHeader_" + current).removeAttribute("class");
  document.getElementById("tabpage_" + current).style.display="none";

  var ident = this.id.split("_")[1];
  //add class of activetabheader to new active tab and show contents
  this.setAttribute("class","tabActiveHeader");
  document.getElementById("tabpage_" + ident).style.display="block";
  this.parentNode.setAttribute("data-current",ident);
}

</script>
</body>
</html>