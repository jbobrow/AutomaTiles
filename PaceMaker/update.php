<?php
        $light = $_GET['light'];
        if($light == "on") {
        $file = fopen("light.json", "w") or die("can't open file");
        fwrite($file, '{"light": "on"}');
        fclose($file);
        }
        else if ($light == "off") {
        $file = fopen("light.json", "w") or die("can't open file");
        fwrite($file, '{"light": "off"}');
        fclose($file);
        }
        ?>

<a href="?light=on">Turn On</a>
<a href="?light=off">Turn Off</a>
<div>
<?php
        if($light=="on") {
        echo("Turn LED on.");
        }
        else if ($light=="off") {
        echo("Turn LED off.");
        }
        else {
        echo ("Do something.");
        }
        ?>
</div>