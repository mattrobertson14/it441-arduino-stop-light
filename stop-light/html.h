const char HOME_page[] PROGMEM = R"=====(
<!DOCTYPE html>
  <html lang="en">
    <head>
      <meta charset="utf-8" />

      <link href="https://fonts.googleapis.com/css?family=Roboto|Roboto+Condensed:300,400" rel="stylesheet">

      <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1" />
      <meta name="theme-color" content="#000000" />
      <meta name="mobile-web-app-capable" content="yes">
      <meta name="apple-mobile-web-app-capable" content="yes">
      
      <title>Stoplight</title>

      <style>
        html, body, #root {
          height: 100%;
          width: 100%;
          margin: 0px;
        }

        :root {
          --red: #FF101F;
          --yellow: #FFCA3A;
          --green: #37FF8B;
          --buttons: #3DA5D9;
          --buttons-hover: #DBEEF8;
        }

        .App {
          font-family: 'Roboto', Arial, Helvetica, sans-serif;
          display: flex;
          flex-direction: column;
          align-items: center;
          background-color: #121213;
          height: 100%;
          width: 100%;
          max-height: 100%;
          color: rgba(255,255,255,.87);
        }

        .stoplight {
          background: rgba(255,255,255,.15);
          padding: 20px;
          width: 120px;
          max-height: 360px;
          flex: 1;
          display: flex;
          flex-direction: column;
          align-items: center;
          justify-content: space-around;
          border-radius: 4px;
        }

        .light {
          display: block;
          border-radius: 100px;
          height: 100px;
          width: 100px;
          outline: none;
          cursor: pointer;
        }

        .light.red {
          background-color: var(--red);
          opacity: .1;
        }

        .light.yellow {
          background-color: var(--yellow);
          opacity: .1;
        }

        .light.green {
          background-color: var(--green);
          opacity: .1;
        }

        .light.on {
          opacity: 1;
        }

        .light:not(.on):hover {
          opacity: .2;
        }

        button {
          margin-top: 16px;
          padding: 8px 16px;
          background-color: transparent;
          color: var(--buttons);
          border: 1px solid var(--buttons);
          border-radius: 4px;
          cursor: pointer;
          outline: none;
          user-select: none;
        }

        button:hover:not(:active) {
          background-color: var(--buttons-hover);
          color: black;
        }

        button:active {
          background-color: var(--buttons);
          color: black;
        }

        @media only screen and (max-width: 599px){
          button:hover:not(:active){
            background-color: initial;
            color: var(--buttons);
          }

          .light:not(.on):hover {
            opacity: .1;
          }
        }
      </style>
      <script type="text/javascript" src="http://code.jquery.com/jquery-1.7.1.min.js"></script>
      <script type="text/javascript">

        $(document).ready(function() {
          checkStatus()
        })
            
        function setLight(color){
          $(`.light:not(.${color})`).removeClass('on');
          $(`.${color}`).addClass('on');        
        }

        function checkStatus(){
          $.ajax({
            url: '/api/status',
            type: "GET",
            success: function(data) {
              setLight(data.color);
              setTimeout(checkStatus, 1000);
            }
          })
        }

        function changeLight(color){
          $.ajax({
            url: `/api/set-${color}`,
            type: "POST",
            success: function(data) {
              setLight(data.color);
            }
          }) 
        }        
      </script>
    </head>
    <body>
      <div id="root">
        <div class="App">
          <h1>Stoplight</h1>
          <div class="stoplight">
            <div class="light red" onclick="changeLight('red')"></div>
            <div class="light yellow" onclick="changeLight('yellow')"></div>
            <div class="light green" onclick="changeLight('green')"></div>    
          </div>
          <button onclick="alert('Turn off lights')">TURN OFF</button>
          <button onclick="alert('Cycling Lights')">AUTO MODE</button>
        </div>
      </div>
    </body>
  </html>  
)=====";
