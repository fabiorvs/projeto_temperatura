<?php
include("banco.php");

$dados['temperatura'] = $_GET['temp'];
$dados['humidade'] = $_GET['hum'];
$dados['ip'] = $_GET['ip'];
$dados['estacao'] = $_GET['estacao'];

$consulta = insertDb($dados,'dados');

echo $consulta;
