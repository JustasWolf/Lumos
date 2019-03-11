/*jslint browser: true, devel: true, node: true */

"use strict";

var xhttp,
	jsonsource;

//function set_value(id, val) {
//	document.getElementById(id).innerHTML = val;
//	console.log("Set to id: " + id + " value: " + val);
//}

function set_value(id, val) {
	document.getElementById(id).value = val;
	console.log("Set to id: " + id + " value: " + val);
}

function set_json_source(s) {
	jsonsource = s;
}

function send(g) {
	xhttp = new XMLHttpRequest();
	xhttp.open("GET", g, true);
	console.log(g);
	xhttp.send();
}

function sw(d) {
	if (document.getElementById(d).checked) {
		send('/light?set=on');
	} else {
		send('/light?set=off');
	}
}

function sin_opt() {
	if (document.getElementById("input_mode").value === "2") {
		document.getElementById("sin_opt").style.display = "block";
	}
	if (document.getElementById("input_mode").value !== "2") {
		document.getElementById("sin_opt").style.display = "none";
	}
}

function loadData(url, callback) {
	xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			callback.apply(xhttp);
		//console.log(xhttp);
		}
	};
	xhttp.open("GET", url, true);
	xhttp.send();
}

function updateData() {
	sin_opt();
	var	js = JSON.parse(xhttp.responseText),
		key;
	console.log("js = " + xhttp.responseText);
	for (key in js) {
		if (js[key].type === 'string') {
			document.getElementById(key).innerHTML = js[key].value;
		}
		if (js[key].type === 'int') {
		  document.getElementById(key + '_value').innerHTML = js[key].value;
			document.getElementById(key + '_value').value = js[key].value;
			document.getElementById('input_' + key).value = js[key].value;
		}
		if (js[key].type === 'bool') {
		  if (js[key].value === 'true') {
		    document.getElementById(key).checked = true;
		  } else {
				if (js[key].value === 'false') {
					document.getElementById(key).checked = false;
	      }
		  }
		}
	}
	
	
}



var upd = setInterval(function () {loadData(jsonsource, updateData); }, 2000);