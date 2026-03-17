fetch('data.txt')
.then(res => res.text())
.then(data => {

    const lines = data.trim().split("\n");
    const chart = document.getElementById("chart");
    const timeline = document.getElementById("timeline");

    let delay = 0;
    let currentTime = 0;
    let waitingTimes = [];

    lines.forEach((line, i) => {

        const [pid, at, bt, ct] = line.split(" ");

        const color = `hsl(${Math.random()*360},70%,50%)`;

        // Gantt Bar
        setTimeout(() => {
            const bar = document.createElement("div");
            bar.className = "bar";
            bar.style.width = (bt * 60) + "px";
            bar.style.background = color;
            bar.innerText = pid;

            chart.appendChild(bar);
        }, delay);

        // Timeline
        const timeMark = document.createElement("span");
        timeMark.innerText = currentTime + "   ";
        timeline.appendChild(timeMark);

        currentTime = ct;

        // Waiting time
        waitingTimes.push(ct - at - bt);

        delay += bt * 600;
    });

    // Final time mark
    const finalMark = document.createElement("span");
    finalMark.innerText = currentTime;
    timeline.appendChild(finalMark);

    // GRAPH
    // 📊 GRAPH (IMPROVED)
setTimeout(() => {

    const canvas = document.getElementById("graph");
    const ctx = canvas.getContext("2d");

    ctx.clearRect(0, 0, canvas.width, canvas.height);

    const baseY = 250;
    const barWidth = 50;
    const gap = 40;

    const maxWT = Math.max(...waitingTimes);

    waitingTimes.forEach((val, i) => {

        if(val < 0) val = 0;

        let height = (val / (maxWT || 1)) * 150;

        let x = i * (barWidth + gap) + 80;
        let y = baseY - height;

        ctx.fillStyle = "#14b8a6";
        ctx.fillRect(x, y, barWidth, height);

        ctx.fillStyle = "black";
        ctx.fillText(lines[i].split(" ")[0], x + 10, baseY + 15);
        ctx.fillText(val, x + 10, y - 5);
    });

    // Y-axis
    ctx.beginPath();
    ctx.moveTo(60, 50);
    ctx.lineTo(60, baseY);
    ctx.stroke();

    // X-axis
    ctx.beginPath();
    ctx.moveTo(60, baseY);
    ctx.lineTo(500, baseY);
    ctx.stroke();

}, delay + 500);
});
