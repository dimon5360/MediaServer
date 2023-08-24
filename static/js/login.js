
function login() {
    const xhr = new XMLHttpRequest();

    console.log("login procedure")

    const json = {
        username: document.getElementById('Username').value,
        password: document.getElementById('Password').value,
    };

    xhr.onreadystatechange = function () {
        if (xhr.status === 200) {
            console.log("got response");
            window.location.href=`/api/v1/main/`;
            return this.responseText;
        }
    };

    xhr.open("POST", "/api/v1/login.js", true);
    xhr.setRequestHeader("Content-Type", "application/json");
    xhr.send(JSON.stringify(json));
}