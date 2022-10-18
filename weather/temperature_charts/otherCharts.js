const charts = {};

charts.plotTemp =  function (temperature) {
        const myDiv = document.getElementById("tempChart")
        var data = [
                {
                        domain: { x: [0, 1], y: [0, 1] },
                        value: temperature,
                        title: { text: "Temperature" },
                        type: "indicator",
                        mode: "gauge+number",
                        gauge: {
                                axis: { range: [null, 50], tickwidth: 5, tickcolor: 'white' },
                                bar: { color: "red", width: 10 },
                                bgcolor: "",
                                borderwidth: 2,
                                steps: [

                                        { range: [0, 10], color: "#42f5f5" },
                                        { range: [10, 25], color: '#42f593' },
                                        { range: [25, 40], color: "#e3f542" },
                                        { range: [40, 50], color: "#f5426c" }
                                ],
                                threshold: {
                                        line: { color: "red", width: 10 },
                                        thickness: 0.75,
                                        value: 40
                                }
                        }
                }
        ];



        var layout = {
                width: innerWidth * (45 / 100), height: innerHeight * (45 / 100),
                margin: { t: 0, b: 0 },
                font: {
                        color: "#f54278",
                },
                paper_bgcolor: "#00203FFF"
        };
        Plotly.newPlot(myDiv, data, layout);
}


const colorObj = {
        cool: "#42f5f5",
        normal: "#42f593",
        hot: "#e3f542",
        extreme: "#f5426c",
}


 




charts.resize =  function () {
        window.addEventListener('resize', function () {
                plotTemp();
        })
};


export default charts;
