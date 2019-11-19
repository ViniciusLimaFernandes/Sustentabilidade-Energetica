var corrente = [];
var firebase = new Firebase('proc-project.firebaseio.com/');
firebase.on('value', function(snapshot) {
	corrente = [];
	for (let i in snapshot.val().corrente) {
		corrente.push(parseFloat(snapshot.val().corrente[i].replace(/,/, '.')));
	}

	drawGraph(corrente, Object.keys(snapshot.val().corrente));

	document.getElementById('potencia').innerText =
		220 * corrente[corrente.length - 1];

	document.getElementById('corrente').innerText =
		corrente[corrente.length - 1];
});

function drawGraph(corrente = [], tempo = []) {
	var labels = [];
	tempo.forEach(t => {
		var data = new Date(parseInt(t));
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
