var corrente = [];
var firebase = new Firebase('pi4-project.firebaseio.com/');
firebase.on('value', function(snapshot) {
	corrente = [];
	for (let i in snapshot.val().corrente) {
		corrente.push(snapshot.val().corrente[i]);
	}

	drawGraph(corrente, Object.keys(snapshot.val().corrente));

	const kwhPrice = 0.25588;
	var potencia = 220 * corrente[corrente.length - 1];
	var corrente = corrente[corrente.length - 1];
	var consumo = (potencia * 24 * 30) / 1000;
	var custo = consumo * kwhPrice;

	document.getElementById('potencia').innerText = potencia.toFixed(2);
	document.getElementById('corrente').innerText = corrente.toFixed(2);
	document.getElementById('consumo').innerText = consumo.toFixed(2);
	document.getElementById('custo').innerText = custo.toFixed(2);
});

function drawGraph(corrente = [], tempo = []) {
	var labels = [];
	tempo.forEach(t => {
		var data = new Date((parseFloat(t)*1000));
		console.log("Data = ", parseFloat(t));
		console.log("Formated Data = ", (parseFloat(t)*1000));
		labels.push(data.toLocaleString());
	});

	var ctx = document.getElementById('currentChart').getContext('2d');
	var myChart = new Chart(ctx, {
		type: 'line',
		data: {
			labels: labels,
			datasets: [
				{
					label: 'Corrente (A)',
					labelString: 'corrente',
					borderColor: 'rgb(255, 99, 132)',
					backgroundColor: 'rgb(255, 99, 132)',
					fill: false,
					data: corrente,
					yAxisID: 'y-axis-corrente'
				}
			]
		},
		options: {
			responsive: true,
			maintainAspectRatio: false,
			hoverMode: 'index',
			stacked: false,
			title: {
				display: true,
				text: 'Corrente',
				fontColor: '#f7f7f7'
			},
			legend: {
				labels: {
					fontColor: '#f7f7f7'
				}
			},
			scales: {
				yAxes: [
					{
						type: 'linear',
						display: true,
						position: 'left',
						id: 'y-axis-corrente',
						ticks: {
							beginAtZero: true,
							suggestedMax: Math.max(...corrente),
							fontColor: '#f7f7f7'
						}
					}
				],
				xAxes: [
					{
						ticks: {
							fontColor: '#f7f7f7'
						}
					}
				]
			}
		}
	});
}
