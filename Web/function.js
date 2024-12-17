// PhongHoc quat 1
let img1 = document.querySelector('#quat11');
function toggleCheck1() {
    if(document.getElementById("myCheckbox1").checked === true){
        img1.src = 'image/fan_on.png'; 
        firebase.database().ref("Outputs").update({F1: 1})
    } else {
        img1.src = 'image/fan_off.png'; 
        firebase.database().ref("Outputs").update({F1: 0})
    }
}

// PhongHoc quat 2
let img2 = document.querySelector('#quat22');
function toggleCheck2() {
    if(document.getElementById("myCheckbox2").checked === true){
        img2.src = 'image/fan_on.png'; 
        firebase.database().ref("Outputs").update({F2: 1})
    } else {
        img2.src = 'image/fan_off.png'; 
        firebase.database().ref("Outputs").update({F2: 0})
    }
}

// PhongHoc den 1
let img3 = document.querySelector('#den11');
function toggleCheck3() {
    if(document.getElementById("myCheckbox3").checked === true){
        img3.src = 'image/led_on.png'; 
        firebase.database().ref("Outputs").update({L1: 1})
    } else {
        img3.src = 'image/led_off.png'; 
        firebase.database().ref("Outputs").update({L1: 0})
    }
}

// PhongHoc den 2
let img4 = document.querySelector('#den22');
function toggleCheck4() {
    if(document.getElementById("myCheckbox4").checked === true){
        img4.src = 'image/led_on.png'; 
        firebase.database().ref("Outputs").update({L2: 1})
    } else {
        img4.src = 'image/led_off.png'; 
        firebase.database().ref("Outputs").update({L2: 0})
    }
}
// PhongHoc den 3
let img5 = document.querySelector('#den33');
function toggleCheck5() {
    if(document.getElementById("myCheckbox5").checked === true){
        img5.src = 'image/led_on.png'; 
        firebase.database().ref("Outputs").update({L3: 1})
    } else {
        img5.src = 'image/led_off.png'; 
        firebase.database().ref("Outputs").update({L3: 0})
    }
}

// PhongHoc den 4
let img6 = document.querySelector('#den44');
function toggleCheck6() {
    if(document.getElementById("myCheckbox6").checked === true){
        img6.src = 'image/led_on.png'; 
        firebase.database().ref("Outputs").update({L4: 1})
    } else {
        img6.src = 'image/led_off.png'; 
        firebase.database().ref("Outputs").update({L4: 0})
    }
}


// Elements for GPIO states
const stateElement1 = document.getElementById("state1");
const stateElement2 = document.getElementById("state2");
const stateElement3 = document.getElementById("state3");
const stateElement4 = document.getElementById("state4");
const stateElement5 = document.getElementById("state5");
const stateElement6 = document.getElementById("state6");
const stateElement7 = document.getElementById("state7");
const stateElement8 = document.getElementById("state8");

// Database path for GPIO states
var dbPathOutput1 = 'Outputs/F1';
var dbPathOutput2 = 'Outputs/F2';
var dbPathOutput3 = 'Outputs/L1';
var dbPathOutput4 = 'Outputs/L2';
var dbPathOutput5 = 'Outputs/L3';
var dbPathOutput6 = 'Outputs/L4';
var dbPathOutput7 = 'Outputs/L5';
var dbPathOutput8 = 'Outputs/Z1';
// Database references
var dbRefOutput1 = firebase.database().ref().child(dbPathOutput1);
var dbRefOutput2 = firebase.database().ref().child(dbPathOutput2);
var dbRefOutput3 = firebase.database().ref().child(dbPathOutput3);
var dbRefOutput4 = firebase.database().ref().child(dbPathOutput4);
var dbRefOutput5 = firebase.database().ref().child(dbPathOutput5);
var dbRefOutput6 = firebase.database().ref().child(dbPathOutput6);
var dbRefOutput7 = firebase.database().ref().child(dbPathOutput7);
var dbRefOutput8 = firebase.database().ref().child(dbPathOutput8);

dbRefOutput1.on('value', snap => {
    if(snap.val()==1) {
        stateElement1.innerText=" ON";
        img1.src = 'image/fan_on.gif'; 
        document.getElementById('myCheckbox1').checked = true;
    }
    else{
        stateElement1.innerText=" OFF";
        img1.src = 'image/fan_off.png'; 
        document.getElementById('myCheckbox1').checked = false;
    }
});

dbRefOutput2.on('value', snap => {
    if(snap.val()==1) {
        stateElement2.innerText=" ON";
        img2.src = 'image/fan_on.gif'; 
        document.getElementById('myCheckbox2').checked = true;
    }
    else{
        stateElement2.innerText=" OFF";
        img2.src = 'image/fan_off.png'; 
        document.getElementById('myCheckbox2').checked = false;
    }
});

dbRefOutput3.on('value', snap => {
    if(snap.val()==1) {
        stateElement3.innerText=" ON";
        img3.src = 'image/led_on.png'; 
        document.getElementById('myCheckbox3').checked = true;
    }
    else{
        stateElement3.innerText=" OFF";
        img3.src = 'image/led_off.png'; 
        document.getElementById('myCheckbox3').checked = false;
    }
});

dbRefOutput4.on('value', snap => {
    if(snap.val()==1) {
        stateElement4.innerText=" ON";
        img4.src = 'image/led_on.png'; 
        document.getElementById('myCheckbox4').checked = true;
    }
    else{
        stateElement4.innerText=" OFF";
        img4.src = 'image/led_off.png'; 
        document.getElementById('myCheckbox4').checked = false;
    }
});

dbRefOutput5.on('value', snap => {
    if(snap.val()==1) {
        stateElement5.innerText=" ON";
        img5.src = 'image/led_on.png'; 
        document.getElementById('myCheckbox5').checked = true;
    }
    else{
        stateElement5.innerText=" OFF";
        img5.src = 'image/led_off.png'; 
        document.getElementById('myCheckbox5').checked = false;
    }
});

dbRefOutput6.on('value', snap => {
    if(snap.val()==1) {
        stateElement6.innerText=" ON";
        img6.src = 'image/led_on.png'; 
        document.getElementById('myCheckbox6').checked = true;
    }
    else{
        stateElement6.innerText=" OFF";
        img6.src = 'image/led_off.png'; 
        document.getElementById('myCheckbox6').checked = false;
    }
});
let img7 = document.querySelector('#den55');
dbRefOutput7.on('value', snap => {
    if(snap.val()==1) {
        stateElement7.innerText=" ON";
        img7.src = 'image/led_on.png'; 
    }
    else{
        stateElement7.innerText=" OFF";
        img7.src = 'image/led_off.png'; 
    }
});
let img8 = document.querySelector('#quat33');
dbRefOutput8.on('value', snap => {
    if(snap.val()==1) {
        stateElement8.innerText=" ON";
        img8.src = 'image/fan_on.gif';
        document.getElementById('excel').src = document.getElementById('excel').src;
    }
    else{
        stateElement8.innerText=" OFF";
        img8.src = 'image/fan_off.png';
        document.getElementById('excel').src = document.getElementById('excel').src;
    }
});