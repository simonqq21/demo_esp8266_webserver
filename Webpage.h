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
                <span class="statusindicator redstatus" name="led1status"></span>
            </div>
            <div class="inputcontainer">
                <input type="button" class="green_btn" value="on" name="led1on" onclick="switchLED(1, 'on');">
                <input type="button" class="red_btn" value="off" name="led1off" onclick="switchLED(1, 'off');">
                <input type="button" class="blue_btn" value="momentary" name="led1moment" onclick="">            
                <input type="button" class="blue_btn" value="blink" name="led1blink" onclick="">
            </div>
        </div>

        <div class="ioctrl">
            <p>LED 2</p>
            <div class="statuscontainer">
                <span>Status:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span>
                <span class="statusindicator redstatus" name="led2status"></span>
            </div>
            <div class="inputcontainer">
                <input type="button" class="green_btn" value="on" name="led2on" onclick="switchLED(2, 'on');">
                <input type="button" class="red_btn" value="off" name="led2off" onclick="switchLED(2, 'off');">
                <input type="button" class="blue_btn" value="momentary" name="led2moment" onclick="">            
                <input type="button" class="blue_btn" value="blink" name="led2blink" onclick="switchLED(2, 'blink');">
            </div>
        </div>

        <div class="ioctrl">
            <p>Button 1</p> 
            <div class="statuscontainer">
                <span>Status:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span>
                <span class="statusindicator redstatus" name="btn1status"></span>
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
        alert(`led/?index=${index}&state=${state}`);
        jhttp.onload = function() {
            let jsondata = json.parse(jhttp.)
            let index = jhttp.
        }
        jhttp.open('GET', `/led/${index}/${state}`);
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