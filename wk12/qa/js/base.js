const canvas = document.getElementById('mazeCanvas')
// const HEIGHT = 400
// const WIDTH = 400
const HEIGHT = 200
const WIDTH = 200
canvas.height = HEIGHT
canvas.width = WIDTH
const cellSize = 20

const ctx = canvas.getContext('2d')
const rows = Math.floor(canvas.height / cellSize)
const cols = Math.floor(canvas.width / cellSize)
const startNode = [0, 0]
const endNode = [rows - 1, cols - 1]

let maze = generateMaze(rows, cols)
const NEIGHBOURS = [
  [0, 1],
  [1, 0],
  [-1, 0],
  [0, -1]
]

const START_COLOR = '#00f'
const END_COLOR = '#f00'
const WALL_COLOR = 'black'
const PATH_COLOR = '#ff0'
const EMPTY_COLOR = 'white'
const GRID_LINE_COLOR = '#fff'

const objectifyForPQ = node => ({ distance: dist(node), node })

const objectifyForAStar = node => ({ distance: total_dist(node), node })

const delay = ms => new Promise(resolve => setTimeout(resolve, ms))

function drawPath (tmp) {
  ctx.fillStyle = PATH_COLOR
  const keys = Object.keys(tmp)
  let maxDistNode = keys
    .map(k => nodify(k))
    .reduce((k, acc) => (dist(k) > dist(acc) ? k : acc), startNode)
  let key = keys.includes(endNode.toString()) ? endNode : maxDistNode
  let path = key != undefined ? [key] : []
  while (key !== startNode && key !== undefined) {
    key = tmp[key.toString()]
    if (key !== undefined) path.push(key)
  }
  path.forEach(n => drawNode(n, PATH_COLOR))
}

const dist = point => (point != undefined ? point[0] + point[1] : 0)
const dist2 = point => point[0] * point[0] + point[1] * point[1]

// const total_dist = (point, end) => dist2(point) + dist2([end[0]-point[0], end[1]-point[1]]);
// only "heuristics" left
const total_dist = point =>
  dist2([endNode[0] - point[0], endNode[1] - point[1]])

function generateMaze (rows, cols) {
  let maze = []
  for (let i = 0; i < rows; i++) {
    maze[i] = []
    for (let j = 0; j < cols; j++) {
      maze[i][j] = Math.random() < 0.32 ? 1 : 0
    }
  }
  maze[0][0] = 0
  maze[rows - 1][rows - 1] = 0
  return maze
}

function drawMaze () {
  ctx.clearRect(0, 0, canvas.width, canvas.height)
  for (let i = 0; i < rows; i++) {
    for (let j = 0; j < cols; j++) {
      ctx.fillStyle = maze[i][j] ? 'black' : 'white'
      ctx.fillRect(j * cellSize, i * cellSize, cellSize, cellSize)
      ctx.strokeStyle = maze[i][j] ? '#000' : '#fff'
      ctx.strokeRect(j * cellSize, i * cellSize, cellSize, cellSize)
    }
  }

  if (startNode) {
    ctx.fillStyle = '#00f'
    ctx.fillRect(
      startNode[1] * cellSize,
      startNode[0] * cellSize,
      cellSize,
      cellSize
    )
  }
  if (endNode) {
    ctx.fillStyle = '#f00'
    ctx.fillRect(
      endNode[1] * cellSize,
      endNode[0] * cellSize,
      cellSize,
      cellSize
    )
  }
}

const isValid = (start, maze) =>
  start[0] >= 0 &&
  start[1] >= 0 &&
  start[0] < maze.length &&
  start[1] < maze[0].length

const isWhite = (node, maze) => maze[node[0]][node[1]] === 0

const drawNode = (node, color = '#0f0') => {
  ctx.fillStyle = color
  ctx.fillRect(node[1] * cellSize, node[0] * cellSize, cellSize, cellSize)
}

const nodify = strNode => {
  if (strNode == undefined) return undefined
  const splits = strNode.split(',')
  return [parseInt(splits[0]), parseInt(splits[1])]
}
