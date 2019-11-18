var myVar = setInterval(atualiza, 1000);
        var TEMP = 0;
        var HUM = 0;
        var EST = "";
        var IP = "";
        var DATA = "";

        var t = new JustGage({
            id: "temperatura",
            value: TEMP,
            min: 0,
            max: 50,
            title: ""
        });


        var h = new JustGage({
            id: "humidade",
            value: HUM,
            min: 0,
            max: 100,
            title: ""
        });

        function atualiza() {

            $.getJSON("api/dados.php", function (result) {
                TEMP = result.temperatura;
                HUM = result.humidade;
                EST = result.estacao;
                IP = result.ip;
                DATA = result.data;
            });

            t.refresh(parseInt(TEMP));
            h.refresh(parseInt(HUM));

            $("#estacao").html(EST);
            $("#ip").html(IP);
            $("#data").html(DATA);
        }