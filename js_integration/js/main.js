function changeOutput(pinToToggle) {
            var someUrl = "/digital_output/toggle";
            $.ajax({
                url: someUrl,
                dataType: "text",
                success: function(response) {
                    if (response == '0') {
                        $("#myImage").attr("src", "pic_bulboff.gif");
                    } else if (response == '1') {
                       $("#myImage").attr("src", "pic_bulbon.gif");
                    }
                },
                timeout: 2000
            })
        }