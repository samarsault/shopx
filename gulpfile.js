var gulp = require('gulp');
var sass = require('gulp-ruby-sass');
// Scripts
var browserSync = require('browser-sync').create();

//
// File Globs
//
var Files = {
	scripts: './js/src/*.js',
	images: './images/src/**/*',
	sass: './styles/main.scss'
};
//
// Compile sass stylesheets
//
gulp.task('sass', function(){
	return sass('./styles/main.scss', {
		style: 'compressed'
	}).pipe(gulp.dest('styles'))
	  .pipe(browserSync.stream());
});

//
// Compress and Minify Scripts
//
gulp.task('js', function () {
	return gulp.src(Files.scripts)
		.pipe(concat('bundle.js'))
		.pipe(gulp.dest('./js/dist/'))
		.pipe(uglify())
		.pipe(gulp.dest('./js/dist/'));
});

//
// Compress Images
//
gulp.task('img-min', function () {
	return gulp.src(Files.images)
	.pipe(imagemin({
		progressive: true,
		svgoPlugins: [{removeViewBox: false}],
		use: [pngquant()]
	}))
	.pipe(gulp.dest('./images/dist/'))
});

//
// Start Browser Sync and File Watcher
//
gulp.task('serve', function () {
	browserSync.init({
		server: './'
	});

	gulp.watch(Files.sass, [ 'sass' ])
	// gulp.watch(Files.scripts,  [ 'js' ]);
	// gulp.watch(Files.images, [ 'img-min' ]);

    // Relaod browser on html and script change
    // gulp.watch('./js/dist/bundle.js').on('change', browserSync.reload);
    gulp.watch('./*.html').on('change', browserSync.reload);
});

//
// Build Site
//
gulp.task('build', [ 'sass', 'js', 'img-min' ]);

//
// Default Task
//
gulp.task('default', ['sass', 'serve']);
