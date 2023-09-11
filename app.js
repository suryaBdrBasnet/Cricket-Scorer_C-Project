import {data} from "./data.js";
let cricketData = new data();

const teamNo1 = document.getElementById("teamNo1");
const scoreData = document.getElementById("scoreData");
const innings = document.getElementById("innings");
const teamNo2 = document.getElementById("teamNo2");
const overData = document.getElementById("overData");


teamNo1.innerText = `${cricketData.team1}`;
scoreData.innerText = `${cricketData.score}-${cricketData.wicket}`;
innings.innerText = cricketData.inning == 1? `1st inning` : `2nd inning`;
teamNo2.innerText = `${cricketData.team2}`;
overData.innerText = `${cricketData.over}.${cricketData.ball}`;