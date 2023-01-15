const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>

<head>
    <meta charset="utf-8">
    <link rel="icon" type="image/jpg"
        href="https://www.cdiscount.com/pdt2/9/3/0/1/700x700/zmagca1269930/rw/machine-a-cafe-a-dosettes-philips-senseo-original.jpg">
    <title>SENSIO</title>

    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body {
            background-color: #000a23;
            font-family: 'Red Hat Display', sans-serif;
            text-align: center;
            -moz-user-select: none;
            /* Firefox */

            -webkit-user-select: none;
            /* Chrome, Safari, Opéra depuis la version 15 */

            -ms-user-select: none;
            /* Internet explorer depuis la version 10 et Edge */
            
            user-select: none;
            /* Propriété standard */

            transition-property: all;
            transition-duration: 0.2s;
        }

        .glow {
            font-size: 80px;
            color: #fff;
            text-align: center;
            animation: glow 1s ease-in-out infinite alternate;
        }

        @-webkit-keyframes glow {
            from {
                text-shadow: 0 0 10px #fff, 0 0 20px #fff, 0 0 30px #e60073, 0 0 40px #e60073, 0 0 50px #e60073, 0 0 60px #e60073, 0 0 70px #e60073;
            }

            to {
                text-shadow: 0 0 30px #fff, 0 0 40px #ff4da6, 0 0 50px #ff4da6, 0 0 60px #ff4da6, 0 0 70px #ff4da6, 0 0 80px #ff4da6, 0 0 90px #ff4da6;
            }
        }

        /*MENU FLEX*/
        .container {
            display: flex;
            width: 300px;
            height: 300px;
            /*justify-content: center;
            align-items: center;*/
            flex-wrap: wrap;
            justify-content: space-around;
            align-content: space-around;
            margin-left: auto;
            margin-right: auto;

            border-radius: 10px;
            background: #000e2e;
            box-shadow: inset 15px 15px 30px #000c26,
                inset -15px -15px 30px #001036;
        }

        .items {
            display: block;
            height: 25%;
            width: 25%;
            margin: 10px;
            text-align: center;

            border-radius: 10px;
            background: #001138;
            box-shadow: 7px 7px 14px #000e2e,
                -7px -7px 14px #001442;

            transition-property: all;
            transition-duration: 0.2s;
        }

        .items:active {
            background: #001138;
            box-shadow: 5px 5px 10px #000e2e,
                -5px -5px 10px #001442;
        }

        img {
            width: 80%;
            height: 80%;
            margin: auto;
            filter: invert(40%);

            transition-property: all;
            transition-duration: 0.1s;
        }

        img:active {
            color: #2dfc52;
            filter: invert(0%) drop-shadow(0 0 2px #2dfc52) drop-shadow(0 0 5px #2dfc52) drop-shadow(0 0 15px #2dfc52);
        }

        #caseTemp {
            color: #5e5e5e;
            font-size: larger;
        }

        #informations {
            color: #5e5e5e;
            width: 43%;
        }

        #erreurs {
            color: #5e5e5e;
            width: 43%;
        }

        span{
            color:white; font-size: x-small;
        }

    </style>

    <script>
        //var i = 0;
        var decompte;
        var enCours = 0

        function send(donneeTrsf) {
            var xhttp = new XMLHttpRequest();
            xhttp.open("GET", "allumage?state=" + donneeTrsf, true);
            xhttp.send();
        }

        setInterval(function () {
            // Call a function repetatively with 1 Second interval
            getTasse();
            getLaTemp();
            getEtat();
            getTouillage();
            /*i++;
            if (i == 150) {
                i = 0;
                clearInterval;
                window.location.reload();
            }*/
        }, 500); //500mSeconds update rate


        function getTasse() {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                  var lesTasses = document.getElementsByClassName('tasses');
                    if (this.responseText == "1") {
                        document.getElementById("erreurs").innerHTML = "Bonne dégustation";
                        document.getElementById("erreurs").style.color = "#5e5e5e";
                        document.body.style.backgroundColor = "#000a23";
                    } else {
                        document.getElementById("erreurs").innerHTML = "<br>Pas de tasse";
                        document.getElementById("erreurs").style.color = "#8a0000";
                        document.body.style.backgroundColor = "#8a0000";
                    }
                }
            };
            xhttp.open("GET", "readTasse", true);
            xhttp.send();
        }

        function getLaTemp() {
            var xhttp = new XMLHttpRequest();
            let coef = 0;
            xhttp.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    document.getElementById("caseTemp").innerHTML = "<br>" +this.responseText;
                }
            };
            xhttp.open("GET", "readTemp", true);
            xhttp.send();
        }

        function getEtat() {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    let boiteAffichage = document.getElementById("informations");
                    let boutonONOFF = document.getElementById("onoffBouton");
                    if (this.responseText == "eteint") {
                        boiteAffichage.innerHTML = "<br>La machine est éteinte";
                        clearInterval(decompte);
                        enCours=0;
                        boutonONOFF.style.filter = "invert(40%)";
                    }
                    else if (this.responseText == "allume" && enCours == 0) {                     
                        var temps = 100;
                        enCours=1;
                        decompte = setInterval(function () {
                            boutonONOFF.style.filter = "invert(40%) drop-shadow(0 0 10px #2dfc52)";
                            boiteAffichage.innerHTML = "<br>Eau en chauffe : " + temps + "s";
                            temps--;
                            if (temps <= 0) {
                                clearInterval(decompte);
                                boiteAffichage.innerHTML = "<br>Machine prête";
                                enCours == 0;
                            }
                        }, 1000);
                    }
                }
            };
            xhttp.open("GET", "readEtat", true);
            xhttp.send();
        }

        function getTouillage() {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    let boutonTouille = document.getElementById("imageTouille");
                    if (this.responseText == "eteint") {
                        boutonTouille.style.filter = "invert(40%)";
                    }
                    else if (this.responseText == "allume") {                     
                        boutonTouille.style.filter = "invert(40%) drop-shadow(0 0 10px #2dfc52)";
                    }
                }
            };
            xhttp.open("GET", "readTouillage", true);
            xhttp.send();
        }

    </script>

    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Red+Hat+Display:ital,wght@1,700&display=swap" rel="stylesheet">
</head>

<body>
    <h1 class="glow">SENSIO</h1>
    <div class="container">
        <div onclick="send(3);" class="items">
            <img id="imageTouille" src="https://img.icons8.com/fluency-systems-regular/96/000000/stir.png" />
            <!--br>
            <span>Mélange</span -->
        </div>
        <div onclick="send(4);" class="items">
            <img id="boutonTemps" src="https://img.icons8.com/windows/128/000000/clock--v3.png" />
            <!--br>
            <span>Quick Caf</span -->
        </div>
        <div id="caseTemp" class="items">
            N/A
        </div>
        <div onclick="send(1)" class="items tasses">
            <img src="https://img.icons8.com/material-outlined/96/000000/espresso-cup--v2.png" />
            <!--br>
            <span>Petite Tasse</span -->
        </div>
        <div onclick="send(0);" class="items">
            <img id="onoffBouton" src="https://img.icons8.com/external-those-icons-fill-those-icons/96/000000/external-power-settings-those-icons-fill-those-icons.png" />
            <!--br>
            <span>Power</span -->
        </div>
        <div onclick="send(2);" class="items tasses">
            <img src="https://img.icons8.com/windows/96/000000/coffee.png" />
            <!--br>
            <span>Grande Tasse</span -->
        </div>
        <div class="items" id="informations"><br>N/A</div>
        <div id="erreurs" class="items"><br>N/A</div>
    </div>

</body>

</html>
)=====";
