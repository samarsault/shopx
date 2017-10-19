/*
* Client Side
*/ 

Vue.use(Buefy.default);

var dashboard = {
	template: '#dashboard-templ',
	data: function (){
		return {
			activeTab: 0,
			cart: [],
			loading: false,
			addItemActive: false,
			name: '',
			desc: '',
			price: '',
			catValue:'',
			inventory:'',
			categories: [ 'Electronics', 'Home' ],
			myItems: []
		}
	},
	mounted: function() {
		if (this.$parent.user != null)
			this.loadItems();
	},
	methods: {
		loadItems: function() {
			var me = this;
			console.log('Loading items..');
			axios.post('/cgi-bin/items', {
				method: 'get',
				item: {
					seller_id: me.$parent.user._id
				}
			}).then(function(resp) {
				me.myItems = resp.data.items;
			}).catch(console.error);
		},
		addItem: function() {
			var form = document.forms.namedItem("imgupload");
			var me = this;
			oData = new FormData(form);
			var xhr = new XMLHttpRequest();
			xhr.open("POST", "/cgi-bin/upload.cgi", true);
			xhr.onload = function(oEvent) {
				if (xhr.status == 200) {
					// success
					var data = JSON.parse(xhr.response);
					if (data.success) {
						// add item
						axios.post('/cgi-bin/items', {
							method: 'add',
							item: {
								name: me.name,
								seller_id: app.user._id,
								category: me.catValue,
								description: me.desc,
								price: parseInt(me.price),
								imageLink: data.url,
								discount: 0, // TODO: Add Discount
								inventory: parseInt(me.inventory)
							}
						}).then(function (resp){
							if (resp.data.success) {
								me.$toast.open('Item Added Successfully..')
							} else {
								me.$toast.open('Error adding item');
							}
						})
						.catch(function (err){
							console.error(err);
						});
					} else {
						me.$toast.open('Error uploading image')
					}
				} else {
				}
			};
			if (app.user == null) {
				this.$toast.open('Sign In First');
				return;
			}
			xhr.send(oData);
		},
		closeModal: function() {
			this.$refs.itemsModal.close();
		}
	}
};

var shop = {
	template: '#shop-templ',
	data: function() {
		return {
			items: [],
			loading: false
		};
	},
	mounted: function() {
		this.load();
	},
	methods:  {
		load: function() {
			var me = this;
			axios.post('/cgi-bin/items', {
				method: 'all'
			}).then(function(resp) {
				if (resp.data.success) {
					me.items = resp.data.items;
				}
			}).catch(console.error);
		}
	}
};

const router = new VueRouter({
	routes: [
		{ path: '/dashboard', component: dashboard },
		{ path: '/shop', component: shop }
	]
});

var app = new Vue({
	el: '#app',
	router: router,
	beforeMount: function() {
		var user = localStorage.getItem('user');
		if (user != null) {
			this.user = JSON.parse(user);
			this.loggedIn = true;
		}
	},
	data: {
		isLoginActive: false,
		loggedIn: false,
		user: null
	}
});

Vue.component('auth', {
	template: '#auth-templ',
	data: function () {
		return {
			register: false,
			email: '',
			pass: '',
			typeOption: 0
		}
	},
	methods: {
		login: function() {
			var me = this;
			if (this.$refs.emailField.isValid) {
				axios.post('/cgi-bin/login/', {
					register: this.register,
					email: this.email,
					pass: this.pass,
					type: this.typeOption - 1
				})
				.then(function (response) {
					if (response.data.success) {
						app.user = response.data.user;
						app.loggedIn = true;
						localStorage.setItem("user", JSON.stringify(app.user));
						me.closeModal();
					}
				})
				.catch(function (error) {
					console.log(error);
				});
			}
		},
		closeModal: function() {
			this.$parent.close();
		}
	}
});