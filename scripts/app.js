Vue.use(Buefy.default,{
	defaultIconPack: 'fa'
});

Vue.component('auth', {
	template: '#auth-templ',
	data: function () {
		return {
			register: false,
			email: '',
			pass: '',
			typeOption:0
		}
	},
	methods: {
		login: function() {
			axios.post('/cgi-bin/login/', {
				register: this.register,
				email: this.email,
				pass: this.pass,
				type: this.typeOption - 1
			})
			.then(function (response) {
				console.log(response.data);
			})
			.catch(function (error) {
				console.log(error);
			});
		},
		closeModal: function() {
			this.$parent.close();
		}
	}
});

var app = new Vue({
	el: '#app',
	data: {
		isLoginActive: false
	}
});