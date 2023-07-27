const char PAGE_MAIN[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width initial-scale=1.0">
    <title></title>
    <style>
        * {
            font-family: Arial, serif;
        }

        header {
            padding: 0.1em 1.5em;
            background: #eeeeff;
        }

        .statuscontainer {
            text-align: left;
            margin-left: 40px;
        }

        .ioctrl {
            margin-bottom: 20px;
        }

        .ioctrl p {
            padding: 0;
            margin: 0;
        }

        .statusindicator {
            width: 20px;
            height: 20px;
            border-radius: 50%;
            display: inline-block;
        }

        .redstatus {
            background-color: red;
        }

        .greenstatus {
            background-color: green;
        }

        .inputcontainer {
            margin-left: 40px;
        }

        input[type="button"] {
            height: 40px;
            min-width: 50px;
            border-radius: 20px;
            margin: 10px 2px 0;
            font-size: 15px;
            color: white;
        }

        .green_btn {
            background-color: green;
        }

        .red_btn {
            background-color: red;
        }

        .blue_btn {
            background-color: blue;
        }
    </style>
</head>  
<body onload="process()">
    <header>
        <h1>ESP8266 Asynchronous Web Server Example</h1>
    </header>
    <div class="ioctrlcontainer">
        <h2>Control Panel</h2>
         
        <div class="ioctrl">
            <p>LED 1</p>
            <div class="statuscontainer">
                <span>Status:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span>
                <span class="statusindicator redstatus" id="led1_status"></span>
            </div>
            <div class="inputcontainer">
                <input type="button" class="green_btn" value="on" id="led1_on" onclick="switchLED(1, 1);">
                <input type="button" class="red_btn" value="off" id="led1_off" onclick="switchLED(1, 0);">
                <input type="button" class="blue_btn" value="momentary" id="led1_moment" onclick="">            
                <input type="button" class="blue_btn" value="blink" id="led1_blink" onclick="">
            </div>
        </div>

    </div>
</body>
<script type="text/javascript">
    let jhttp = new XMLHttpRequest();

/*
URL passed from browser to uc
update LED state 
/led/${index}/${state}

JSON passed from uc to browser
respond with actual LED state
{
    type: "led",
    index: ${index},
    state: ${state}
}
respond with actual button state 
{
    type: "button",
    index: ${index},
    state: ${state}
}

*/
    // index is the index number of the LED
    // state can be 'on', 'off', or 'blink'
    function switchLED(index, state) {
        let jhttp = new XMLHttpRequest();
        jhttp.onload = function() {
            let jsondata = JSON.parse(jhttp.responseText);
            let index = jsondata.index;
            let state = jsondata.state;
            statusElement = document.getElementById(`led${index}_status`);
            if (state <= 0) {
                statusElement.classList.remove("greenstatus");
                statusElement.classList.add("redstatus");
            }
            else {
                statusElement.classList.remove("redstatus");
                statusElement.classList.add("greenstatus");
            }

        }
        jhttp.open('GET', `/led?index=${index}&state=${state}`);
        jhttp.send(); 
    }

    /* function to turn on the LED when the button is
    pressed, and turn off the LED when the button is released */
    function momentaryLED(index, state) {
    
    }

    function response() {
        let message; 
        let jsonresponse = jhttp.responseText; 


    }

    function process() {
    
    }
</script>
</html>
)=====";